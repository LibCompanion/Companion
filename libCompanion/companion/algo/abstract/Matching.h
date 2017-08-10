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

#ifndef COMPANION_ABSTRACTFEATUREMATCHING_H
#define COMPANION_ABSTRACTFEATUREMATCHING_H

#include "ImageRecognition.h"

#include <companion/draw/Frame.h>
#include <companion/model/processing/FeatureMatchingModel.h>

namespace Companion {

    namespace Algorithm {

        /**
         * Abstract matching class implementation for all default matching algorithms like feature matching or template matching.
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS Matching : public ImageRecognition {

        public:

            /**
             * Get current scene model.
             * @return Scene model to process in matching algorithm.
             */
            Model::Processing::ImageRecognitionModel *getSceneModel() const;

            /**
             * Sets given model as scene.
             * @param sceneModel Scene model to set.
             */
            void setSceneModel(Model::Processing::ImageRecognitionModel *sceneModel);

            /**
             * Get object model.
             * @return Object model for a matching algorithm.
             */
            Model::Processing::ImageRecognitionModel *getObjectModel() const;

            /**
             * Sets given object model.
             * @param objectModel Object model to set.
             */
            void setObjectModel(Model::Processing::ImageRecognitionModel *objectModel);

            /**
             * Feature matching algorithm implementation.
             * @throws Companion::Error::Code If an error occured in search operation.
             */
            virtual Companion::Model::Result* executeAlgorithm() = 0;

            /**
             * Indicator if this algorithm use cuda.
             * @return True if cuda will be used otherwise false.
             */
            virtual bool isCuda() = 0;

        protected:

            /**
             * Current scene model to verify for matching.
             */
            Model::Processing::ImageRecognitionModel *sceneModel;

            /**
             * Current object model to verify for matching.
             */
            Model::Processing::ImageRecognitionModel *objectModel;
        };
    }
}

#endif //COMPANION_ABSTRACTFEATUREMATCHING_H
