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
#include <companion/util/exportapi/ExportAPIDefinitions.h>

namespace Companion {

    /**
     * Enumeration class for color formats.
     */
    enum class COMP_EXPORTS ColorFormat {
        RGB,
        RGBA,
        BGR,
        BGRA,
        GRAY
    };

    /**
     * Utility class for opencv.
     * @author Andreas Sekulksi
     */
    class COMP_EXPORTS Util {

    public:

        /**
         * Checks if given image is loaded.
         * @param img Image to check if it's not null.
         * @return <code>true</code> if image is loaded otherwise <code>false</code>
         */
        static bool isImageLoaded(cv::Mat &img);

        /**
         * Resize given image.
         * @param img Image to resize.
         * @param size_x Resize image to given width in pixels.
         */
        static void resizeImage(cv::Mat &img, int size_x);

        /**
         * Ratio to set new point from scaled frame.
         * @param point Point to calculate new positions.
         * @param cWidth Current width.
         * @param cHeight Current height.
         * @param nWidth New width.
         * @param nHeight New height.
         */
        static void ratioPosition(cv::Point &point, int cWidth, int cHeight, int nWidth, int nHeight);

        /**
         * Checks if a point is distant enough from a given origin.
         * @param origin Origin point.
         * @param point Point whose distance should be checked.
         * @param distance Minimum distance.
         * @return <code>true</code> if point is distant enough otherwise <code>false</code>
         */
        static bool hasDistantPosition(cv::Point2f origin, cv::Point2f point, int distance);

        /**
         * Converts an image to the given color format. BGR is expected as source format.
         * @param src Source image.
         * @param dst Destination image.
         * @param colorFormat Color format for the converted image.
         */
        static void convertColor(cv::Mat& src, cv::Mat& dst, ColorFormat colorFormat);
    };

}

#endif //COMPANION_UTIL_H
