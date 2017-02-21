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

#ifndef COMPANION_CFEATUREMATCHING_H
#define COMPANION_CFEATUREMATCHING_H

#include <opencv2/core/core.hpp>
#include <opencv2/cudafeatures2d.hpp>

#include "companion/algo/AbstractFeatureMatching.h"

class CFeatureMatching : public AbstractFeatureMatching {

public:

    /**
     * Constructor to create an cuda based feature matching.
     * @param cudaFeatureMatching Cuda based feature matching algorithm like cv::cuda::ORB.
     */
    CFeatureMatching(cv::Ptr<cv::Feature2D> cudaFeatureMatching);

    /**
     * Destructor
     */
    virtual ~CFeatureMatching();

    /**
     * Cuda implementation from feature matching.
     * @param searchModel Search model to compare.
     * @param compareModel Compare model to check if contains in search model.
     * @throws CompanionError::error_code If an error occured in search operation.
     */
    virtual Drawable* algo(ImageRecognitionModel *searchModel, ImageRecognitionModel *compareModel);

private:

    /**
     * Cuda feature matching algorithm.
     */
    cv::Ptr<cv::Feature2D> cudaFeatureMatching;

};

#endif //COMPANION_CFEATUREMATCHING_H