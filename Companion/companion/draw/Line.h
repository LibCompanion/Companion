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

#ifndef COMPANION_LINE_H
#define COMPANION_LINE_H

#include "Drawable.h"

#include <companion/util/CompanionError.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <companion/util/Util.h>

namespace Companion {
	namespace Draw
	{
		/**
		 * Class implementation for a drawable line.
		 * @author Andreas Sekulski, Dimitri Kotlovsky
		 */
		class COMP_EXPORTS Line : public Drawable
		{

		public:

			/**
			 * Constructor to create a drawable line.
			 * @param start Starting position of the line.
			 * @param end Ending position of the line.
			 * @param color Color of the line.
			 * @param thickness Thickness of the line.
			 */
			Line(cv::Point start,
				cv::Point end,
				cv::Scalar color = cv::Scalar(0.0, 255.0, 0.0, 255.0),
				int thickness = 4);

			/**
			 * Draw this line to the given image.
			 * @param image Image on which to draw the object.
			 */
			virtual void Draw(cv::Mat image);

			/**
			 * Scale this line from the current image size to a new image size.
			 * @param cWidth Current image width.
			 * @param cHeight Current image height.
			 * @param nWidth New image width.
			 * @param nHeight New image height.
			 */
			virtual void Ratio(int cWidth, int cHeight, int nWidth, int nHeight);

			/**
			 * Get cut area around this line.
			 * @return Cut area around this line as a cv::Rect.
			 */
			virtual cv::Rect CutArea();

			/**
			 * Obtain the origin's x coordinate.
			 * @return X coordinate of the line's origin.
			 */
			virtual int OriginX();

			/**
			 * Obtain the origin's y coordinate.
			 * @return Y coordinate of the line's origin.
			 */
			virtual int OriginY();

			/**
			 * Move the line's origin.
			 * @param x Relative distance to move the origin on the x axis.
			 * @param y Relative distance to move the origin on the y axis.
			 */
			virtual void MoveOrigin(int x, int y);

			/**
			 * Set line color.
			 * @param color Line color to set.
			 */
			virtual void Color(const cv::Scalar& color);

			/**
			 * Get the color of the line.
			 * @return Line color.
			 */
			virtual const cv::Scalar& Color() const;

			/**
			 * Set thickness of the line.
			 * @param thickness Line thickness.
			 */
			virtual void Thickness(int thickness);

			/**
			 * Get thickness of this line, if 0 no line will be drawn.
			 * @return Line thickness.
			 */
			virtual int Thickness() const;

			/**
			 * Get starting point of this line.
			 * @return Starting point of this line.
			 */
			const cv::Point& Start() const;

			/**
			 * Set a given starting point for this line.
			 * @param start Starting point to set for this line.
			 */
			void Start(const cv::Point& start);

			/**
			 * Get endpoint of this line.
			 * @return Endpoint of this line.
			 */
			const cv::Point& End() const;

			/**
			 * Set a given endpoint for this line.
			 * @param end Endpoint to set for this line.
			 */
			void End(const cv::Point& end);

		private:

			/**
			 * Starting point of this line.
			 */
			cv::Point start;

			/**
			 * Endpoint of this line.
			 */
			cv::Point end;

			/**
			 * Line color.
			 */
			cv::Scalar color;

			/**
			 * Line thickness.
			 */
			int thickness;
		};
	}
}

#endif //COMPANION_LINE_H
