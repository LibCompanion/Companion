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

#ifndef COMPANION_IMAGERECOGNITION_H
#define COMPANION_IMAGERECOGNITION_H

#include <companion/model/processing/ImageRecognitionModel.h>
#include <companion/model/result/Result.h>

namespace Companion {

    namespace Algorithm {

        /**
         * Image recognition abstract class to implement specific image recognition algorithms.
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS ImageRecognition {

        public:

            /**
             * Specific algorithm implementation like simple compare or template matching.
             * @param searchModel Search model to compare.
             * @param compareModel Compare model to check if contains in search model.
             * @throws Companion::Error::Code If an error occured in search operation.
             */
            virtual Companion::Model::Result* algo(Companion::Model::Processing::ImageRecognitionModel *searchModel,
                                                   Companion::Model::Processing::ImageRecognitionModel *compareModel) = 0;

            /**
             * Indicator if this algorithm use cuda.
             * @return True if cuda will be used otherwise false for CPU/OpenCL usage.
             */
            virtual bool isCuda() = 0;
        };
    }
}



#endif //COMPANION_IMAGERECOGNITION_H