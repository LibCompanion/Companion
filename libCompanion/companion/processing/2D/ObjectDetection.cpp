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

#include "ObjectDetection.h"

Companion::Processing::ObjectDetection::ObjectDetection(Companion::Configuration *companion,
                                                        Algorithm::ImageRecognition *imageRecognition,
                                                        float scale) {
    this->companion = companion;
    this->imageRecognition = imageRecognition;
    this->scale = scale;
}

Companion::Processing::ObjectDetection::~ObjectDetection() {
    
}

CALLBACK_RESULT Companion::Processing::ObjectDetection::execute(cv::Mat frame) {

    Model::Processing::FeatureMatchingModel *scene;
    Companion::Model::Result *result;
    CALLBACK_RESULT objects;
    std::vector<Model::Processing::ImageRecognitionModel*> models;

    if (!frame.empty()) {
        scene = new Model::Processing::FeatureMatchingModel();
        models = companion->getModels();

        int oldX = frame.cols;
        int oldY = frame.rows;

        // https://antifreezedesign.wordpress.com/2011/05/13/permutations-of-1920x1080-for-perfect-scaling-at-1-77/
        Util::resizeImage(frame, oldX * scale, oldY * scale);
        scene->setImage(frame);

        // Check if image recognition implementation is an cuda implementation.
        if(imageRecognition->isCuda()) {
            // Cuda usage -> Don't use multithreading
            for(unsigned long x = 0; x < models.size(); x++) {
                Companion::Model::Processing::ImageRecognitionModel *model = models.at(x);
                result = imageRecognition->algo(scene, model);
                if(result != nullptr) {
                    // Create old image size
                    result->getModel()->ratio(frame.cols, frame.rows, oldX, oldY);
                    // Store detected object and its ID to vector.
                    objects.push_back(result);
                }
            }
        } else {
            // Multithreading will be used for CPU usage
            // ToDo Currently disabled because of an racing condition to use models...
            //#pragma omp parallel for
            for(unsigned long x = 0; x < models.size(); x++) {
                Companion::Model::Processing::ImageRecognitionModel *model = models.at(x);
                result = imageRecognition->algo(scene, model);
                if(result != nullptr) {
                    // Create old image size
                    result->getModel()->ratio(frame.cols, frame.rows, oldX, oldY);
                    // Store detected object and its ID to vector.
                    objects.push_back(result);
                }
            }
        }

        frame.release();
        delete scene;
    }

    return objects;
}