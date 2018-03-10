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

#include "HybridRecognition.h"

Companion::Processing::Recognition::HybridRecognition::HybridRecognition(Companion::Processing::Recognition::HashRecognition *hashRecognition,
    Companion::Algorithm::Recognition::Matching::Matching *featureMatching, int resize)
{
    this->hashRecognition = hashRecognition;
    this->featureMatching = featureMatching;
    this->resize = resize;

    // Disable features from specific algorithms
    if(dynamic_cast<Companion::Algorithm::Recognition::Matching::FeatureMatching*>(this->featureMatching) != nullptr) {
        // Disable IRA because this is not used by hybrid recognition because ROI's are from hash recognition.
        dynamic_cast<Companion::Algorithm::Recognition::Matching::FeatureMatching*>(this->featureMatching)->setUseIRA(false);
    }
}

Companion::Processing::Recognition::HybridRecognition::~HybridRecognition()
{
}

void Companion::Processing::Recognition::HybridRecognition::addModel(cv::Mat image, int id)
{
    Companion::Model::Processing::FeatureMatchingModel *model = new Companion::Model::Processing::FeatureMatchingModel();
    model->setID(id);
    model->setImage(image);
    this->models[id] = model;
    this->hashRecognition->addModel(id, image);
}

void Companion::Processing::Recognition::HybridRecognition::removeModel(int modelID)
{
    this->models.erase(modelID);
    // ToDo delete method for hashRecognition...
}

void Companion::Processing::Recognition::HybridRecognition::clearModels()
{
    this->models.clear();
    // ToDo delete method for hashRecognition...
}

CALLBACK_RESULT Companion::Processing::Recognition::HybridRecognition::execute(cv::Mat frame)
{
    CALLBACK_RESULT results;
    std::vector<CALLBACK_RESULT> parallelizedResults;
    cv::Mat roiFrame;
    std::vector<Companion::Error::Code> errors;
    Companion::Model::Result::RecognitionResult* hashResult;
    std::vector<Companion::Model::Result::Result*> hashResults;
    int threads;

    threads = omp_get_max_threads();
    parallelizedResults = std::vector<CALLBACK_RESULT>(threads);
    hashResults = this->hashRecognition->execute(frame);

    if (!hashResults.empty())
    {
        errors.clear();
        #pragma omp parallel for
        for (int i = 0; i < hashResults.size(); i++)
        {
            try
            {
                hashResult = dynamic_cast<Companion::Model::Result::RecognitionResult*>(hashResults.at(i));
                if (hashResult != nullptr)
                {
                    processing(hashResult, frame, parallelizedResults[omp_get_thread_num()]);
                }
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

    #pragma omp critical
    for (int i = 0; i < threads; i++)
    {
        for (int j = 0; j < parallelizedResults[i].size(); j++)
        {
            results.push_back(dynamic_cast<Companion::Model::Result::Result*>(parallelizedResults[i].at(j)));
        }
    }
   
    return results;
}

void Companion::Processing::Recognition::HybridRecognition::processing(
        Companion::Model::Result::RecognitionResult* hashResult,
        cv::Mat frame,
        CALLBACK_RESULT &results)
{
    
    cv::Mat cutImage;
    Companion::Draw::Drawable *cutDrawable;
    Companion::Model::Result::RecognitionResult *fmResult;
    Companion::Model::Processing::FeatureMatchingModel *sceneModel;
    int oldX, oldY;

    // This frame can be cut to improve recognition
    cutDrawable = hashResult->getDrawable();
    cutImage = Companion::Util::cutImage(frame, cutDrawable->getCutArea());

    oldX = cutImage.cols;
    oldY = cutImage.rows;

    if(this->resize != 100)
    {
        Util::resizeImage(cutImage, cutImage.cols * this->resize / 100);
    }

    sceneModel = new Companion::Model::Processing::FeatureMatchingModel();
    sceneModel->setImage(cutImage);   

    fmResult = this->featureMatching->executeAlgorithm(sceneModel, this->models[hashResult->getId()], nullptr);
    
    if (fmResult != nullptr)
    {
        fmResult->getDrawable()->ratio(cutImage.cols, cutImage.rows, oldX, oldY);
        fmResult->getDrawable()->moveOrigin(cutDrawable->getOriginX(), cutDrawable->getOriginY());
        results.push_back(dynamic_cast<Companion::Model::Result::Result*>(fmResult));
    }

    delete sceneModel;
}
