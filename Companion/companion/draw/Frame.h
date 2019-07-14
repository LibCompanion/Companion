/*
 * This program is an object recognition framework written with OpenCV.
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

#ifndef COMPANION_FRAME_H
#define COMPANION_FRAME_H

#include "Drawable.h"
#include "Line.h"

#include <opencv2/core/core.hpp>
#include <memory>

namespace Companion {
	namespace Draw
	{
		/**
		 * Implementation class for a drawable frame.
		 * @author Andreas Sekulski, Dimitri Kotlovsky
		 */
		class COMP_EXPORTS Frame : public Drawable
		{

		public:

			/**
			 * Constructor to create a drawable frame.
			 * @param topLeft Top left position.
			 * @param topRight Top right position.
			 * @param bottomLeft Bottom left position.
			 * @param bottomRight Bottom right position.
			 * @param color Color of the frame.
			 * @param thickness Thickness of the frame.
			 */
			Frame(cv::Point topLeft,
				cv::Point topRight,
				cv::Point bottomLeft,
				cv::Point bottomRight,
				cv::Scalar color = cv::Scalar(0.0, 255.0, 0.0, 255.0),
				int thickness = 4);

			/**
			 * Destructor.
			 */
			virtual ~Frame() = default;

			/**
			 * Draw this frame to the given image.
			 * @param image Image on which to draw the object.
			 */
			virtual void Draw(cv::Mat image);

			/**
			 * Scale this frame from the current image size to a new image size.
			 * @param cWidth Current image width.
			 * @param cHeight Current image height.
			 * @param nWidth New image width.
			 * @param nHeight New image height.
			 */
			virtual void Ratio(int cWidth, int cHeight, int nWidth, int nHeight);

			/**
			 * Obtain the origin's x coordinate.
			 * @return X coordinate of the frames's origin.
			 */
			virtual int OriginX();

			/**
			 * Obtain the origin's y coordinate.
			 * @return Y coordinate of the frames's origin.
			 */
			virtual int OriginY();

			/**
			 * Move the frame's origin.
			 * @param x Relative distance to move the origin on the x axis.
			 * @param y Relative distance to move the origin on the y axis.
			 */
			virtual void MoveOrigin(int x, int y);

			/**
			 * Set frame color.
			 * @param color Frame color to set.
			 */
			virtual void Color(const cv::Scalar& color);

			/**
			 * Get cut area around this frame.
			 * @return Cut area around this frame as a cv::Rect.
			 */
			virtual cv::Rect CutArea();

			/**
			 * Get the color of the frame.
			 * @return Frame color.
			 */
			virtual const cv::Scalar& Color() const;

			/**
			 * Get thickness of the frame, if 0 no frame will be drawn.
			 * @return Frame thickness.
			 */
			virtual int Thickness() const;

			/**
			 * Set thickness of the frame.
			 * @param thickness Frame thickness.
			 */
			virtual void Thickness(int thickness);

			/**
			 * Set top left corner of the frame.
			 * @param topLeft Top left corner.
			 */
			void TopLeft(const cv::Point& topLeft);

			/**
			 * Set top right corner of the frame.
			 * @param topRight Top right corner.
			 */
			void TopRight(const cv::Point& topRight);

			/**
			 * Set bottom left corner of the frame.
			 * @param bottomLeft Bottom left corner.
			 */
			void BottomLeft(const cv::Point& bottomLeft);

			/**
			 * Set bottom right corner of the frame.
			 * @param bottomRight Bottom right corner.
			 */
			void BottomRight(const cv::Point& bottomRight);

			/**
			 * Get top left corner of the frame.
			 * @return Top left corner.
			 */
			const cv::Point& TopLeft() const;

			/**
			 * Get top right corner of the frame.
			 * @return Top right corner.
			 */
			const cv::Point& TopRight() const;

			/**
			 * Get bottom left corner of the frame.
			 * @return Bottom left corner.
			 */
			const cv::Point& BottomLeft() const;

			/**
			 * Get bottom right corner of the frame.
			 * @return Bottom right corner.
			 */
			const cv::Point& BottomRight() const;

		private:

			/**
			 * Top left corner of the frame.
			 */
			cv::Point topLeft;

			/**
			 * Top right corner of the frame.
			 */
			cv::Point topRight;

			/**
			 * Bottom left corner of the frame.
			 */
			cv::Point bottomLeft;

			/**
			 * Bottom right corner of the frame.
			 */
			cv::Point bottomRight;

			/**
			 * Color of the frame.
			 */
			cv::Scalar color;

			/**
			 * Thickness of the frame.
			 */
			int thickness;
		};
	}
}

#endif //COMPANION_FRAME_H
