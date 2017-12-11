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

#ifndef COMPANION_HYBRIDDETECTION_H
#define COMPANION_HYBRIDDETECTION_H

#include <companion/processing/ImageProcessing.h>
#include <companion/processing/HashDetection.h>
#include <companion/algo/matching/Matching.h>
#include <companion/algo/matching/FeatureMatching.h>
#include <companion/algo/detection/ShapeDetection.h>
#include <companion/model/Result.h>
#include <companion/model/processing/FeatureMatchingModel.h>

namespace Companion { namespace Processing
{
    class HybridDetection : public ImageProcessing {

        public:

            HybridDetection(Companion::Processing::HashDetection *hashDetection,
                Companion::Algorithm::Matching::Matching *featureMatching);

            /**
             * Destructor
             */
            virtual ~HybridDetection();

            /**
             * Add searching model type to search.
             * @param model Model to search.
             * @param id Identifier from model.
             */
            void addModel(cv::Mat image, int id);

            /**
            * Deletes given model if exists. This method can only safely used if searching process is not RUNNING!.
            * @param model Model to delete.
            * @throws Companion::Error::Code Companion error code if currently search is using.
            */
            void removeModel(int id);

            /**
            * Clear all models which are searched.
            */
            void clearModels();

            CALLBACK_RESULT execute(cv::Mat frame);

        private:

            Companion::Processing::HashDetection *hashDetection;

            Companion::Algorithm::Matching::Matching *featureMatching;

            std::map<int, Companion::Model::Processing::FeatureMatchingModel*> models;




        };
}}


#endif //COMPANION_HYBRIDDETECTION_H
