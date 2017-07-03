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
#include <companion/model/processing/ImageRecognitionModel.h>
#include <companion/model/processing/FeatureMatchingModel.h>
#include <companion/draw/Drawable.h>
#include <companion/algo/abstract/ImageRecognition.h>
#include <companion/Configuration.h>

namespace Companion {

    namespace Processing {

        /**
         * 2D Object detection implementation.
         *
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS ObjectDetection : public ImageProcessing {

        public:

            /**
             * Constructor to create an object detection algorithm implementation.
             * @param companion Configuration class to obtain model entities to verify.
             * @param imageRecognition Image recognition algorithm to use, for example feature matching.
             * @param scale Scaling factor from frame. Default by one.
             */
            ObjectDetection(Companion::Configuration *companion, Algorithm::ImageRecognition *imageRecognition, float scale = 1);

            /**
             * Destructor
             */
            virtual ~ObjectDetection();

            /**
             * Try to detect all objects from give frame.
             * @param frame Frame to check for an object location.
             * @return  An empty vector if no objects are detected or otherwise a pair of a Drawable and the ID for
             *          every detected object.
             */
            virtual CALLBACK_RESULT execute(cv::Mat frame);

        private:

            /**
             * Scaling factor from image to resize. Decrease frame size < Default (1) > Increase frame size
             */
            float scale;

            /**
             * Companion configuration which contains model data to search.
             */
            Companion::Configuration *companion;

            /**
             * FeatureMatching algorithm setup.
             */
            Algorithm::ImageRecognition *imageRecognition;
        };

    }

}

#endif //COMPANION_OBJECTDETECTION_H