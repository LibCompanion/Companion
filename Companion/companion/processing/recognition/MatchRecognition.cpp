/*
 * This program is an object recognition framework written with OpenCV.
 * Copyright (C) 2016-2018 Andreas Sekulski, Dimitri Kotlovsky
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MatchRecognition.h"

Companion::Processing::Recognition::MatchRecognition::MatchRecognition(PTR_MATCHING_RECOGNITION matchingAlgo,
    Companion::SCALING scaling,
	PTR_SHAPE_DETECTION shapeDetection)
{
    this->matchingAlgo = matchingAlgo;
    this->scaling = scaling;
    this->shapeDetection = shapeDetection;
}

CALLBACK_RESULT Companion::Processing::Recognition::MatchRecognition::Execute(cv::Mat frame)
{
    CALLBACK_RESULT results;
    std::vector<CALLBACK_RESULT> parallelizedResults;
	PTR_FEATURE_MATCHING featureMatching;
	PTR_MODEL_FEATURE_MATCHING sceneModel = std::make_shared<MODEL_FEATURE_MATCHING>();;
    std::vector<PTR_DRAW_FRAME> rois;
    std::vector<Companion::Error::Code> errors;
    int oldX, oldY, threads;

    // Create vector result list to parallelize
    if (this->matchingAlgo->IsCuda())
    {
        threads = 1;
        parallelizedResults = std::vector<CALLBACK_RESULT>(threads);
    }
    else
    {
        threads = omp_get_max_threads();
        parallelizedResults = std::vector<CALLBACK_RESULT>(threads);
    }

    if (!frame.empty())
    {
        oldX = frame.cols;
        oldY = frame.rows;

        // Shrink the image with a given scale factor or a given output width. Use this list for good 16:9 image sizes:
        // https://antifreezedesign.wordpress.com/2011/05/13/permutations-of-1920x1080-for-perfect-scaling-at-1-77/
        Util::ResizeImage(frame, this->scaling);
        sceneModel->Image(frame);

        featureMatching = std::dynamic_pointer_cast<FEATURE_MATCHING>(this->matchingAlgo);
        if (featureMatching != nullptr)
        {
            // Matching algorithm is feature matching
            // Pre calculate full image scene model keypoints
            featureMatching->CalculateKeyPoints(sceneModel);
        }

        if (this->shapeDetection != nullptr)
        {
            // If shape detection should be used obtain all possible ROIs from frame
            rois = this->shapeDetection->ExecuteAlgorithm(sceneModel->Image());
        }

        if (this->matchingAlgo->IsCuda())
        {
            for (size_t x = 0; x < models.size(); x++)
            {
                Processing(sceneModel,
                    models.at(x),
                    rois,
                    frame,
                    oldX,
                    oldY,
                    parallelizedResults[omp_get_thread_num()]);
            }
        }
        else
        {
            errors.clear();
            #pragma omp parallel for
            for (int x = 0; x < models.size(); x++)
            {
                try
                {
                    Processing(sceneModel,
                        models.at(x),
                        rois,
                        frame,
                        oldX,
                        oldY,
                        parallelizedResults[omp_get_thread_num()]);
                }
                catch (Companion::Error::Code errorCode)
                {
                    #pragma omp critical
                    errors.push_back(errorCode);
                }
            }

            if (!errors.empty())
            {
                throw Companion::Error::CompanionException(errors);
            }
        }

        frame.release();
    }

    #pragma omp critical
    for (int i = 0; i < threads; i++) 
    {
        for (int j = 0; j < parallelizedResults[i].size(); j++) 
        {
            results.push_back(parallelizedResults[i].at(j));
        }
    }

    return results;
}

void Companion::Processing::Recognition::MatchRecognition::Processing(PTR_MODEL_FEATURE_MATCHING sceneModel,
	PTR_MODEL_FEATURE_MATCHING objectModel,
    std::vector<PTR_DRAW_FRAME> rois,
    cv::Mat frame,
    int originalX,
    int originalY,
    CALLBACK_RESULT &results)
{
	PTR_RESULT result = nullptr;

    if (!objectModel)
    {
        // If wrong model types are used
        throw Companion::Error::Code::wrong_model_type;
    }

    if (rois.size() == 0)
    {
        try 
        {
            // If ROIs not found or used
            result = std::shared_ptr<RESULT>(this->matchingAlgo->ExecuteAlgorithm(sceneModel, objectModel, nullptr));
        }
        catch (Companion::Error::Code errorCode)
        {
            throw errorCode;
        }
    }
    else
    {
        size_t index = 0;
        // If ROIs found
        while (index < rois.size())
        {
            try 
            {
                result = std::shared_ptr<RESULT>(this->matchingAlgo->ExecuteAlgorithm(sceneModel, objectModel, rois.at(index)));
            }
            catch (Companion::Error::Code errorCode)
            {
                throw errorCode;
            }
            index++;
        }
    }

    if (result != nullptr)
    {
        // Create old image size
        result->Drawable()->Ratio(frame.cols, frame.rows, originalX, originalY);
        // Store recognized object and its ID to vector.
        results.push_back(result);
    }
}

bool Companion::Processing::Recognition::MatchRecognition::AddModel(PTR_MODEL_FEATURE_MATCHING model)
{

    if (!model->Image().empty())
    {
        this->models.push_back(model);
        return true;
    }

    return false;
}

bool Companion::Processing::Recognition::MatchRecognition::RemoveModel(int modelID)
{
    for (size_t index = 0; index < this->models.size(); index++)
    {
        if (this->models.at(index)->ID() == modelID) {
            this->models.erase(this->models.begin() + index);
            return true;
        }
    }
    return false;
}

void Companion::Processing::Recognition::MatchRecognition::ClearModels()
{
    this->models.clear();
}
