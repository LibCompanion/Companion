/*
* This program is an image recognition library written with OpenCV.
* Copyright (C) 2016-2017 Andreas Sekulski
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

#include "HybridDetection.h"

Companion::Processing::HybridDetection::HybridDetection(Companion::Processing::HashDetection *hashDetection,
    Companion::Algorithm::Matching::Matching *featureMatching, int resize)
{
    this->hashDetection = hashDetection;
    this->featureMatching = featureMatching;
    this->resize = resize;
}

Companion::Processing::HybridDetection::~HybridDetection()
{
}

void Companion::Processing::HybridDetection::addModel(cv::Mat image, int id)
{
    Companion::Model::Processing::FeatureMatchingModel *model = new Companion::Model::Processing::FeatureMatchingModel();
    model->setID(id);
    model->setImage(image);
    models[id] = model;
    hashDetection->addModel(id, image);
}

void Companion::Processing::HybridDetection::removeModel(int id)
{
    models.erase(id);
    // ToDo delete method for hashDetection...
}

void Companion::Processing::HybridDetection::clearModels()
{
    models.clear();
    // ToDo delete method for hashDetection...
}

CALLBACK_RESULT Companion::Processing::HybridDetection::execute(cv::Mat frame)
{
    CALLBACK_RESULT cbResults;
    std::vector<Companion::Model::Result*> results;
    Companion::Model::Result* hashResult;
    std::vector<Companion::Model::Result*> hashResults;
    Companion::Model::Result *fmResult;
    cv::Mat roiFrame;
    int modelID;
    Companion::Model::Processing::FeatureMatchingModel *sceneModel;
    Companion::Model::Processing::FeatureMatchingModel *objectModel;

    hashResults = hashDetection->execute(frame);

    if (!hashResults.empty())
    {
        sceneModel = new Companion::Model::Processing::FeatureMatchingModel();

        #pragma omp parallel for
        for (int i = 0; i < hashResults.size(); i++)
        {
            hashResult = hashResults.at(i);
            modelID = hashResult->getId();

            // This frame can be cut to improve detection
            Companion::Draw::Drawable *cutDrawable = hashResult->getModel();
            cv::Mat cutImage = Companion::Util::cutImage(frame, cutDrawable->cutArea());

            int oldX = cutImage.cols;
            int oldY = cutImage.rows;
            if(resize != 100)
            {
                Util::resizeImage(cutImage, cutImage.cols * resize / 100);
            }

            sceneModel->setImage(cutImage);

            objectModel = models[modelID];
            objectModel->getIra()->clear();

            fmResult = featureMatching->executeAlgorithm(sceneModel, objectModel, nullptr);

            if (fmResult != nullptr)
            {
                fmResult->getModel()->ratio(cutImage.cols, cutImage.rows, oldX, oldY);
                fmResult->getModel()->moveGroundZero(cutDrawable->getGroundZeroX(), cutDrawable->getGroundZeroY());
                cbResults.push_back(fmResult);
            }

        }

        delete sceneModel;
    }
   
    return cbResults;
}
