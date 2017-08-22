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
#include <companion/algo/2D/FeatureMatching.h>
#include <companion/algo/2D/ShapeDetection.h>
#include <companion/Configuration.h>

namespace Companion 
{

    namespace Processing 
	{

        /**
         * 2D Object detection implementation.
         *
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS ObjectDetection : public ImageProcessing 
		{

        public:

            /**
             * Constructor to create an object detection algorithm implementation.
             * @param companion Configuration class to obtain model entities to verify.
             * @param matchingAlgo Image recognition algorithm to use, for example feature matching.
             * @param scaling Scaling to resize an image. Default is full hd.
			 * @param shapeDetection Shape detection algorithm to detect roi's in images optional if not set complete image will be searched.
             */
            ObjectDetection(Companion::Configuration *companion,
                            Companion::Algorithm::Matching *matchingAlgo,
                            Companion::SCALING scaling = Companion::SCALING::SCALE_1920x1080,
							Companion::Algorithm::ShapeDetection *shapeDetection = nullptr);

            /**
             * Destructor
             */
            virtual ~ObjectDetection();

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
             * Companion configuration which contains model data to search.
             */
            Companion::Configuration *companion;

            /**
             * Matching algorithm.
             */
            Companion::Algorithm::Matching *matchingAlgo;

			/**
			 * Shape detection algorithm.
			 */
			Companion::Algorithm::ShapeDetection *shapeDetection;
        };

    }

}

#endif //COMPANION_OBJECTDETECTION_H