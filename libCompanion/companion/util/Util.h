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

#include "companion/util/exportapi/ExportAPIDefinitions.h"

namespace Companion {

    /**
     * Utility class for opencv.
     * @author Andreas Sekulksi
     */
    class COMP_EXPORTS Util {

    public:

        /**
         * Checks if given image is loaded.
         * @param img Image to check if it's not null.
         * @return <b>TRUE</b> if image is loaded otherwise <b>False<b>
         */
        static bool isImageLoaded(cv::Mat &img);

        /**
         * Resize given image.
         * @param img Image to resize.
         * @param size_x Resize image to given height in pixel.
         * @param size_y Resize image to given width in pixel.
         */
        static void resizeImage(cv::Mat &img, int size_x, int size_y);

        /**
         * Ratio to set new point from scaled frame.
         * @param point Point to calculate new positions.
         * @param cWidth Current width.
         * @param cHeight Current height.
         * @param nWidth New width.
         * @param nHeight New height.
         */
        static void ratioPosition(cv::Point &point, int cWidth, int cHeight, int nWidth, int nHeight);
    };

}

#endif //COMPANION_UTIL_H