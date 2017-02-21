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

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>

#include "companion/draw/Drawable.h"
#include "companion/util/CompanionError.h"
#include "companion/model/FeatureMatchingModel.h"
#include "companion/util/Util.h"

/**
 * Image recognition abstract class to implement specific image recognition algorithms.
 * @author Andreas Sekulski
 */
class ImageRecognition {

public:

    /**
     * Specific algorithm implementation like simple compare or template matching.
     * @param searchModel Search model to compare.
     * @param compareModel Compare model to check if contains in search model.
     * @throws CompanionError::error_code If an error occured in search operation.
     */
    virtual Drawable* algo(ImageRecognitionModel *searchModel, ImageRecognitionModel *compareModel) = 0;

    /**
     * Indicator if this algorithm use cuda.
     * @return True if cuda will be used otherwise false.
     */
    virtual bool isCuda() = 0;
};

#endif //COMPANION_IMAGERECOGNITION_H