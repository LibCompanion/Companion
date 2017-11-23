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

namespace Companion
{

	/**
	 * Enumeration class for color formats.
	 */
	enum class COMP_EXPORTS ColorFormat
	{
		RGB,
		RGBA,
		BGR,
		BGRA,
		GRAY
	};

	/**
	 * Enumeration class for scaling.
	 */
	enum class COMP_EXPORTS SCALING
	{
		SCALE_2048x1152,
		SCALE_1920x1080,
		SCALE_1600x900,
		SCALE_1408x792,
		SCALE_1344x756,
		SCALE_1280x720,
		SCALE_1152x648,
		SCALE_1024x576,
		SCALE_960x540,
		SCALE_896x504,
		SCALE_800x450,
		SCALE_768x432,
		SCALE_640x360
	};

	/**
	 * Utility class for opencv.
	 * @author Andreas Sekulksi
	 */
	class COMP_EXPORTS Util
	{

	public:

		/**
		 * Checks if given image is loaded.
		 * @param img Image to check if it's not null.
		 * @return <code>true</code> if image is loaded, <code>false</code> otherwise
		 */
		static bool isImageLoaded(cv::Mat &img);

		/**
		 * Resize given image.
		 * @param img Image to resize.
		 * @param scaling Scaling factor to resize.
		 */
		static void resizeImage(cv::Mat &img, SCALING scaling);

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
         * Checks diagonals from detected points to check if result is a valid rectangle.
         * @param topRight Top right detected position from object.
         * @param bottomLeft Bottom left detected position from object.
         * @param topLeft Top left detected position from object.
         * @param bottomRight Bottom right detected position from object.
         * @param threshold Threshold in 0 to 100 percentage to validate rectangle.
         * @param distance Minimum distance from corners.
         * @return <code>true</code> if point is distant enough otherwise <code>false</code>
         */
        static bool checkDistantDiagonals(cv::Point2f topRight, cv::Point2f bottomLeft, cv::Point2f topLeft, cv::Point2f bottomRight, int threshold, int distance);

		/**
		 * Converts an image to the given color format. BGR is expected as source format.
		 * @param src Source image.
		 * @param dst Destination image.
		 * @param colorFormat Color format for the converted image.
		 */
		static void convertColor(cv::Mat& src, cv::Mat& dst, ColorFormat colorFormat);

	private:

		static cv::Point getScaling(SCALING scaling);
	};

}

#endif //COMPANION_UTIL_H
