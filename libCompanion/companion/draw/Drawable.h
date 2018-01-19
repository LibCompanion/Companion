/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2018 Andreas Sekulski
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

namespace Companion { namespace Draw
{
	/**
	 * Abstract class implementation for an area.
	 * @author Andreas Sekulski.
	 */
	class COMP_EXPORTS Drawable
	{

	public:
		/**
		 * Draw given object to image.
		 * @param image Image to draw an object.
		 */
		virtual void draw(cv::Mat image) = 0;

		/**
		 * Ratio to scale position from drawable to new location.
		 * @param cWidth Current width.
		 * @param cHeight Current height.
		 * @param nWidth New width.
		 * @param nHeight New height.
		 */
		virtual void ratio(int cWidth, int cHeight, int nWidth, int nHeight) = 0;

        /**
         * Obtain cut area from drawable.
         * @return Cut area as rect for a cut frame.
         */
        virtual cv::Rect cutArea() = 0;

        /**
         * Obtain origin x position.
         * @return X coordinate from drawable object.
         */
		virtual int getOriginX() = 0;

        /**
         * Obtain origin y position.
         * @return Y coordinate from drawable object.
         */
		virtual int getOriginY() = 0;

        /**
         * Move from origin drawable.
         * @param x X vector to move.
         * @param y Y vector to move.
         */
		virtual void moveOrigin(int x, int y) = 0;

        /**
         * Set color from drawable.
         * @param color Color to set.
         */
        virtual void setColor(const cv::Scalar &color) = 0;

        /**
         * Get color from drawable.
         * @return Scalar color from frame.
         */
        virtual const cv::Scalar &getColor() const = 0;

        /**
         * Get thickness size from drawable.
         * @return Get thickness size from frame.
         */
        virtual int getThickness() const = 0;

        /**
          * Set thickness size from drawable.
          * @param thickness Thickness size.
          */
        virtual void setThickness(int thickness) = 0;

	};
}}

#endif //COMPANION_DRAWABLE_H
