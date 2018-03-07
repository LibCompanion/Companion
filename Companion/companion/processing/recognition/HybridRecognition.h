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

#ifndef COMPANION_HYBRIDRECOGNITION_H
#define COMPANION_HYBRIDRECOGNITION_H

#include <companion/processing/ImageProcessing.h>
#include <companion/processing/recognition/HashRecognition.h>
#include <companion/algo/detection/ShapeDetection.h>
#include <companion/algo/recognition/matching/FeatureMatching.h>
#include <companion/model/processing/FeatureMatchingModel.h>
#include <companion/util/CompanionException.h>
#include <omp.h>

namespace Companion { namespace Processing { namespace Recognition
{
    /**
     * Hybrid recognition implementation to recognize objects based on hash values and verify with a matching alogrithm (for example feature matching).
     * @author Andreas Sekulski
     */
    class HybridRecognition : public ImageProcessing
    {

    public:

        /**
         * Hybrid recognition constructor.
         * @param hashRecognition Hash recognition to use.
         * @param featureMatching Feature matching to verify recognized hashes.
         * @param resize Resize image factor from 1 to 100. 100 original scale 1 is 99 percentage smaller image.
         */
        HybridRecognition(Companion::Processing::Recognition::HashRecognition *hashRecognition,
            Companion::Algorithm::Recognition::Matching::Matching *featureMatching,
            int resize = 100);

        /**
         * Default destructor.
         */
        virtual ~HybridRecognition();

        /**
         * Add searching model type to search.
         * @param model Model to search.
         * @param id Identifier from model.
         */
        void addModel(cv::Mat image, int id);

        /**
         * Removes given model if it exists. This method can only be used safetly if the searching process is not running.
         * @param modelID ID of the model to remove.
         */
        void removeModel(int modelID);

        /**
         * Clear all models which are searched.
         */
        void clearModels();

        /**
         * Try to recognize all objects in the given frame.
         * @param frame Frame to check for an object location.
         * @return  An empty vector if no objects are recognized or otherwise a pair of a Drawable and the ID for
         *          every recognized object.
         */
        CALLBACK_RESULT execute(cv::Mat frame);

    private:

        /**
         * Resize factor for recognized hash models images.
         */
        int resize;

        /**
         * Hash recognition to use to search for rois.
         */
        Companion::Processing::Recognition::HashRecognition *hashRecognition;

        /**
         * Feature matching algorithm to use for model verification.
         */
        Companion::Algorithm::Recognition::Matching::Matching *featureMatching;

        /**
         * A map from all models to recognize.
         */
        std::map<int, Companion::Model::Processing::FeatureMatchingModel*> models;

        /**
         * Processing method to recognize objects.
         * @param hashResult Result from hash recognition.
         * @param frame Scene frame.
         * @param results Objects list from all recognized objects.
         */
        void processing(Companion::Model::Result::RecognitionResult* hashResult,
                        cv::Mat frame,
                        CALLBACK_RESULT &results);

        };
}}}

#endif //COMPANION_HYBRIDRECOGNITION_H
