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
#include "companion/util/exportapi/ExportAPIDefinitions.h" 

namespace Companion {

    namespace Algorithm {

        namespace Cuda {

            /**
             * Feature matching cuda implementation class.
             *
             * @author Andreas Sekulski
             */
            class COMP_EXPORTS FeatureMatching : public AbstractFeatureMatching {

            public:

                /**
                 * Constructor to create an cuda based feature matching.
                 * @param cudaFeatureMatching Cuda based feature matching algorithm like cv::cuda::ORB.
                 * @param cornerDistance How many pixels the corners of a found area should be distant from each other. Default value is 10.
                 */
                FeatureMatching(cv::Ptr<cv::Feature2D> cudaFeatureMatching, int cornerDistance = 10);

                /**
                 * Destructor
                 */
                virtual ~FeatureMatching();

                /**
                 * Cuda implementation from feature matching.
                 * @param sceneModel Scene model from image recognition search, must be from model class <b>FeatureMatchingModel</b>.
                 * @param objectModel Object model from image recognition search, must be from model class <b>FeatureMatchingModel</b>.
                 * @throws Companion::Error::Code If an error occurred in search operation.
                 */
                virtual Companion::Model::Result* algo(Model::Processing::ImageRecognitionModel *sceneModel,
                                                       Model::Processing::ImageRecognitionModel *objectModel);

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