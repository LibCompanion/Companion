/*
* This program is an image recognition library written with OpenCV.
* Copyright (C) 2016-2018 Andreas Sekulski
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
    CALLBACK_RESULT cbResults;
    cv::Mat roiFrame;
    std::vector<Companion::Error::Code> errors;
    Companion::Model::Result::RecognitionResult* hashResult;
    std::vector<Companion::Model::Result::Result*> hashResults;

    hashResults = this->hashRecognition->execute(frame);

    if (!hashResults.empty())
    {
        errors.clear();
        #pragma omp parallel for
        for (int i = 0; i < hashResults.size(); i++)
        {
            try
            {
                hashResult = dynamic_cast<Companion::Model::Result::RecognitionResult*>(hashResults.at(i % hashResults.size()));
                if (hashResult != nullptr)
                {
                    processing(hashResult, frame, cbResults);
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
   
    return cbResults;
}

void Companion::Processing::Recognition::HybridRecognition::processing(
        Companion::Model::Result::RecognitionResult* hashResult,
        cv::Mat frame,
        CALLBACK_RESULT &cbResults)
{
    Companion::Model::Result::RecognitionResult *fmResult;
    int modelID = hashResult->getId();
    Companion::Model::Processing::FeatureMatchingModel *sceneModel = new Companion::Model::Processing::FeatureMatchingModel();
    Companion::Model::Processing::FeatureMatchingModel *objectModel;

    // This frame can be cut to improve recognition
    Companion::Draw::Drawable *cutDrawable = hashResult->getDrawable();
    cv::Mat cutImage = Companion::Util::cutImage(frame, cutDrawable->cutArea());

    int oldX = cutImage.cols;
    int oldY = cutImage.rows;
    if(this->resize != 100)
    {
        Util::resizeImage(cutImage, cutImage.cols * this->resize / 100);
    }

    sceneModel->setImage(cutImage);

    objectModel = this->models[modelID];
    objectModel->getIra()->clear();

    fmResult = this->featureMatching->executeAlgorithm(sceneModel, objectModel, nullptr);
    delete sceneModel;

    if (fmResult != nullptr)
    {
        fmResult->getDrawable()->ratio(cutImage.cols, cutImage.rows, oldX, oldY);
        fmResult->getDrawable()->moveOrigin(cutDrawable->getOriginX(), cutDrawable->getOriginY());
        #pragma omp critical
        cbResults.push_back(dynamic_cast<Companion::Model::Result::Result*>(fmResult));
    }
}
