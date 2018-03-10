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

#ifndef COMPANION_MATCHRECOGNITION_H
#define COMPANION_MATCHRECOGNITION_H

#include <opencv2/core/core.hpp>
#include <companion/processing/ImageProcessing.h>
#include <companion/model/processing/FeatureMatchingModel.h>
#include <companion/draw/Drawable.h>
#include <companion/util/CompanionException.h>
#include <companion/algo/recognition/matching/FeatureMatching.h>
#include <companion/algo/detection/ShapeDetection.h>
#include <companion/Configuration.h>
#include <omp.h>

namespace Companion { namespace Processing { namespace Recognition
{
    /**
     * Match recognition implementation to recognize objects based on matching algorithms.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS MatchRecognition : public ImageProcessing
    {

    public:

        /**
         * Match recognition constructor.
         * @param matchingAlgo Matching algorithm to use, for example feature matching.
         * @param scaling Scaling to resize an image. Default is 1920x1080.
         * @param shapeDetection Shape detection algorithm to detect ROI's in images (if not set the whole image will be searched).
         */
        MatchRecognition(Companion::Algorithm::Recognition::Matching::Matching *matchingAlgo,
            Companion::SCALING scaling = Companion::SCALING::SCALE_1920x1080,
            Companion::Algorithm::Detection::ShapeDetection *shapeDetection = nullptr);

        /**
         * Destructor.
         */
        virtual ~MatchRecognition();

        /**
         * Add search model type to search for.
         * @param model Model to search for.
         * @return <code>True</code> if model is added, <code>false</code> otherwise.
         */
        bool addModel(Companion::Model::Processing::FeatureMatchingModel *model);

        /**
         * Remove given model if it exists. This method can only be used safely if the searching process is not running.
         * @param modelID ID of the model to remove.
         * @return <code>True</code> if the model was deleted, otherwise <code>false</code>.
         */
        bool removeModel(int modelID);

        /**
         * Clear all models which are searched for.
         */
        void clearModels();

        /**
         * Try to recognize all objects in the given frame.
         * @param frame Frame to check for an object location.
         * @return A vector of results for the given frame or an empty vector if no objects are recognized.
         */
        CALLBACK_RESULT execute(cv::Mat frame);

    private:

        /**
         * Scaling value to resize image.
         */
        Companion::SCALING scaling;

        /**
         * Matching algorithm.
         */
        Companion::Algorithm::Recognition::Matching::Matching *matchingAlgo;

        /**
         * Shape detection algorithm.
         */
        Companion::Algorithm::Detection::ShapeDetection *shapeDetection;

        /**
         * Feature matching models.
         */
        std::vector<Companion::Model::Processing::FeatureMatchingModel*> models;

        /**
         * Processing method to recognize objects.
         * @param sceneModel Scene model to check.
         * @param objectModel Object model to search in scene.
         * @param rois List of ROIs if existent.
         * @param frame Scene frame.
         * @param originalX Original width of the scene frame.
         * @param originalY Original height of the scene frame.
         * @param results List of all recognized objects.
         */
        void processing(Model::Processing::FeatureMatchingModel* sceneModel,
            Model::Processing::FeatureMatchingModel* objectModel,
            std::vector<Companion::Draw::Frame*> rois,
            cv::Mat frame,
            int originalX,
            int originalY,
            CALLBACK_RESULT &results);
    };
}}}

#endif //COMPANION_MATCHRECOGNITION_H
