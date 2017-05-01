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

#ifndef COMPANION_CUDAFEATUREMATCHING_H
#define COMPANION_CUDAFEATUREMATCHING_H

#include <opencv2/core/core.hpp>
#include <opencv2/cudafeatures2d.hpp>

#include "companion/algo/abstract/AbstractFeatureMatching.h"

namespace Companion {

    namespace Algorithm {

        namespace Cuda {

            /**
             * Feature matching cuda implementation class.
             *
             * @author Andreas Sekulski
             */
            class FeatureMatching : public AbstractFeatureMatching {

            public:

                /**
                 * Constructor to create an cuda based feature matching.
                 * @param cudaFeatureMatching Cuda based feature matching algorithm like cv::cuda::ORB.
                 */
                FeatureMatching(cv::Ptr<cv::Feature2D> cudaFeatureMatching);

                /**
                 * Destructor
                 */
                virtual ~FeatureMatching();

                /**
                 * Cuda implementation from feature matching.
                 * @param searchModel Search model to compare.
                 * @param compareModel Compare model to check if contains in search model.
                 * @throws Companion::Error::Code If an error occurred in search operation.
                 */
                virtual Draw::Drawable* algo(Model::ImageRecognitionModel *searchModel,
                                             Model::ImageRecognitionModel *compareModel);

                /**
                 * Indicator if this algorithm use cuda.
                 * @return True if cuda will be used otherwise false.
                 */
                virtual bool isCuda();

            private:

                /**
                 * Cuda feature matching algorithm.
                 */
                cv::Ptr<cv::Feature2D> cudaFeatureMatching;

            };
        }
    }
}

#endif //COMPANION_CUDAFEATUREMATCHING_H