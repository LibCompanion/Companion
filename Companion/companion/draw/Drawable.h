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

#ifndef COMPANION_DRAWABLE_H
#define COMPANION_DRAWABLE_H

#include <opencv2/core/core.hpp>
#include <companion/util/exportapi/ExportAPIDefinitions.h>

namespace Companion {
	namespace Draw
	{
		/**
		 * Abstract class for a drawable object.
		 * @author Andreas Sekulski, Dimitri Kotlovsky
		 */
		class COMP_EXPORTS Drawable
		{

		public:

			/**
			 * Draw this drawable object to the given image.
			 * @param image Image on which to draw the object.
			 */
			virtual void Draw(cv::Mat image) = 0;

			/**
			 * Scale this drawable from the current image dimensions to new image dimensions.
			 * @param cWidth Current image width.
			 * @param cHeight Current image height.
			 * @param nWidth New image width.
			 * @param nHeight New image height.
			 */
			virtual void Ratio(int cWidth, int cHeight, int nWidth, int nHeight) = 0;

			/**
			 * Get cut area around this drawable.
			 * @return Cut area around this drawable as a cv::Rect.
			 */
			virtual cv::Rect CutArea() = 0;

			/**
			 * Obtain the origin's x coordinate.
			 * @return X coordinate of the drawable object's origin.
			 */
			virtual int OriginX() = 0;

			/**
			 * Obtain the origin's y coordinate.
			 * @return Y coordinate of the drawable object's origin.
			 */
			virtual int OriginY() = 0;

			/**
			 * Move the drawable object's origin.
			 * @param x Relative distance to move the origin on the x axis.
			 * @param y Relative distance to move the origin on the y axis.
			 */
			virtual void MoveOrigin(int x, int y) = 0;

			/**
			 * Set drawable color.
			 * @param color Drawable color to set.
			 */
			virtual void Color(const cv::Scalar& color) = 0;

			/**
			 * Get the color of the drawable.
			 * @return Drawable color.
			 */
			virtual const cv::Scalar& Color() const = 0;

			/**
			 * Get thickness of this drawable.
			 * @return Drawable thickness.
			 */
			virtual int Thickness() const = 0;

			/**
			 * Set thickness of the drawable.
			 * @param thickness Drawable thickness.
			 */
			virtual void Thickness(int thickness) = 0;

		};
	}
}

#endif //COMPANION_DRAWABLE_H
