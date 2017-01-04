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

#ifndef COMPANION_UTIL_H
#define COMPANION_UTIL_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "companion/openCV3/util/CompanionError.h"

/**
 * Utility class for opencv.
 * @author Andreas Sekulksi
 */
class Util {

public:

    /**
     * Checks if given image is loaded.
     * @param img Image to check if it's not null.
     * @return <b>TRUE</b> if image is loaded otherwise <b>False<b>
     */
    static bool is_image_loaded(cv::Mat &img);

    /**
     * @brief Gets from given error code an corresponding error message.
     * @param code The error code to get corresponding error message.
     * @return String message from given error.
     */
    static std::string get_error(CompanionError::error_code code);

    /**
     * Resize given image.
     * @param img Image to resize.
     * @param size_x Resize image to x px.
     * @param size_y  Resize image to y px.
     */
    static void resize_image(cv::Mat &img, int size_x, int size_y);
};

#endif //COMPANION_UTIL_H