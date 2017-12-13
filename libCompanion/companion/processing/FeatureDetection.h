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

#ifndef COMPANION_OBJECTDETECTION_H
#define COMPANION_OBJECTDETECTION_H

#include <opencv2/core/core.hpp>
#include <companion/processing/ImageProcessing.h>
#include <companion/model/processing/FeatureMatchingModel.h>
#include <companion/draw/Drawable.h>
#include <companion/algo/matching/FeatureMatching.h>
#include <companion/algo/detection/ShapeDetection.h>
#include <companion/Configuration.h>
#include <omp.h>

namespace Companion { namespace Processing
{

    /**
     * Feature matching detection implementation.
     * @author Andreas Sekulski
     */
    class COMP_EXPORTS FeatureDetection : public ImageProcessing
    {

    public:

        /**
         * Constructor to create an object detection algorithm implementation.
         * @param matchingAlgo Image recognition algorithm to use, for example feature matching.
         * @param scaling Scaling to resize an image. Default is full hd.
         * @param shapeDetection Shape detection algorithm to detect roi's in images optional if not set complete image will be searched.
         */
        FeatureDetection(Companion::Algorithm::Matching::Matching *matchingAlgo,
            Companion::SCALING scaling = Companion::SCALING::SCALE_1920x1080,
            Companion::Algorithm::Detection::ShapeDetection *shapeDetection = nullptr);

        /**
         * Add searching model type. For example an object (feature) which should be detected.
         * @param model Model to search.
         * @return True if model is added false if not.
         */
        bool addModel(Companion::Model::Processing::FeatureMatchingModel *model);

        /**
         * Deletes given model if exists. This method can only safely used if searching process is not RUNNING!.
         * @param model Model to delete.
         * @throws Companion::Error::Code Companion error code if currently search is using.
         * @return True if model deleted otherwise false.
         */
        bool removeModel(Companion::Model::Processing::FeatureMatchingModel *model);

        /**
         * Clear all models which are searched.
         */
        void clearModels();

        /**
         * Destructor
         */
        virtual ~FeatureDetection();

        /**
         * Try to detect all objects from given frame.
         * @param frame Frame to check for an object location.
         * @return  An empty vector if no objects are detected or otherwise a pair of a Drawable and the ID for
         *          every detected object.
         */
        virtual CALLBACK_RESULT execute(cv::Mat frame);

    private:

        /**
         * Scaling enumerator to resize image.
         */
        Companion::SCALING scaling;

        /**
         * Matching algorithm.
         */
        Companion::Algorithm::Matching::Matching *matchingAlgo;

        /**
         * Shape detection algorithm.
         */
        Companion::Algorithm::Detection::ShapeDetection *shapeDetection;

        /**
         * Feature matching models.
         */
        std::vector<Companion::Model::Processing::FeatureMatchingModel*> models;

        /**
         * Processing method to detect objects.
         * @param sceneModel Scene model to check.
         * @param objectModel Object model to search in scene.
         * @param rois Region of interests list if exists.
         * @param frame Scene frame.
         * @param originalX Original x size from frame.
         * @param originalY Original y size from frame.
         * @param objects Objects list from all detected objects.
         */
        void processing(Model::Processing::FeatureMatchingModel* sceneModel,
            Model::Processing::FeatureMatchingModel* objectModel,
            std::vector<Companion::Draw::Frame*> rois,
            cv::Mat frame,
            int originalX,
            int originalY,
            CALLBACK_RESULT &objects);
    };
}}

#endif //COMPANION_OBJECTDETECTION_H