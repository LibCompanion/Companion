/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2018 Andreas Sekulski, Dimitri Kotlovsky
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

 /// @file
#ifndef COMPANION_UTIL_H
#define COMPANION_UTIL_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <companion/model/result/Result.h>
#include <companion/util/exportapi/ExportAPIDefinitions.h>

namespace Companion
{
	/**
	 * Color formats.
	 */
	enum class ColorFormat
	{
		RGB, ///< RGB color format.
		RGBA, ///< RGBA color format.
		BGR, ///< BGR color format.
		BGRA, ///< BGRA color format.
		GRAY ///< GRAY color format.
	};

	/**
	 * Scaling resolutions.
	 */
	enum class SCALING
	{
		SCALE_2048x1152, ///< Scaling resolution of 2048 x 1152 pixels.
		SCALE_1920x1080, ///< Scaling resolution of 1920 x 1080 pixels.
		SCALE_1600x900, ///< Scaling resolution of 1600 x 900 pixels.
		SCALE_1408x792, ///< Scaling resolution of 1408 x 792 pixels.
		SCALE_1344x756, ///< Scaling resolution of 1344 x 756 pixels.
		SCALE_1280x720, ///< Scaling resolution of 1280 x 720 pixels.
		SCALE_1152x648, ///< Scaling resolution of 1152 x 648 pixels.
		SCALE_1024x576, ///< Scaling resolution of 1024 x 576 pixels.
		SCALE_960x540, ///< Scaling resolution of 960 x 540 pixels.
		SCALE_896x504, ///< Scaling resolution of 896 x 504 pixels.
		SCALE_800x450, ///< Scaling resolution of 800 x 450 pixels.
		SCALE_768x432, ///< Scaling resolution of 768 x 432 pixels.
		SCALE_640x360, ///< Scaling resolution of 640 x 360 pixels.
		SCALE_320x180 ///< Scaling resolution of 320 x 180 pixels.
	};

	/**
	 * Utility class for Companion.
	 * @author Andreas Sekulksi, Dimitri Kotlovsky
	 */
	class COMP_EXPORTS Util
	{

	public:

		/**
		 * Cut the given area out of the given image.
		 * @param img Source image.
		 * @param cutArea Area to cut out.
		 * @return Cut out image.
		 */
		static cv::Mat CutImage(cv::Mat img, cv::Rect cutArea);

		/**
		 * Check if given image is loaded.
		 * @param img Image to check if it is not null.
		 * @return <code>True</code> if image is loaded, <code>false</code> otherwise.
		 */
		static bool IsImageLoaded(const cv::Mat& img);

		/**
		 * Resize given image.
		 * @param img Image to resize.
		 * @param scaling Scaling factor to resize.
		 */
		static void ResizeImage(cv::Mat& img, SCALING scaling);

		/**
		 * Resize given image.
		 * @param img Image to resize.
		 * @param size Width and height in pixels to resize image.
		 */
		static void ResizeImage(cv::Mat& img, cv::Size size);

		/**
		 * Resize given image to given width (keeps aspect ratio).
		 * @param img Image to resize.
		 * @param width Image width in pixels.
		 */
		static void ResizeImage(cv::Mat& img, int width);

		/**
		 * Recalculate the coordinates of the given point from current image dimensions to new given dimensions.
		 * @param point Point the coordinates of which should be recalculated.
		 * @param cWidth Current width.
		 * @param cHeight Current height.
		 * @param nWidth New width.
		 * @param nHeight New height.
		 */
		static void RatioPosition(cv::Point& point, int cWidth, int cHeight, int nWidth, int nHeight);

		/**
		 * Validate the rectangular shape of a recognized area.
		 * @param topRight Top right corner of the recognized area.
		 * @param bottomLeft Bottom left corner of the recognized area.
		 * @param topLeft Top left corner of the recognized area.
		 * @param bottomRight Bottom right corner of the recognized area.
		 * @param minSidelLength Minimum length of the recognized area's sides (in pixels).
		 * @param maxSideDeviation Maximum deviation of the sides' lengths.
		 * @param maxDiagonalDeviation Maximum deviation of the diagonals' lengths.
		 * @return <code>True</code> if the diagonals are long enough and both have a similar length, otherwise <code>false</code>.
		 */
		static bool ValidateShape(cv::Point2f topRight, cv::Point2f bottomLeft, cv::Point2f topLeft, cv::Point2f bottomRight, int minSidelLength = 10, float maxSideDeviation = 0.5f, float maxDiagonalDeviation = 0.1f);

		/**
		 * Convert an image to the given color format. BGR is expected as source format.
		 * @param src Source image.
		 * @param dst Destination image.
		 * @param colorFormat Color format for the converted image.
		 */
		static void ConvertColor(cv::Mat& src, cv::Mat& dst, ColorFormat colorFormat);

	private:

		/**
		 * Get scaling from given enumeration as point representation.
		 * @param scaling Scaling to obtain point.
		 * @return Scaling as point representation where x is the width and y the height in pixels.
		 */
		static cv::Point Scaling(SCALING scaling);

		/**
		 * Calculate the deviation between two lengths.
		 * @param x First length.
		 * @param y Second length.
		 * return Deviation in percent.
		 */
		static double Deviation(double x, double y);
	};
}

#endif //COMPANION_UTIL_H
