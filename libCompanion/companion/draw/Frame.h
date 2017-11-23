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

#ifndef COMPANION_LINES_H
#define COMPANION_LINES_H

#include "Drawable.h"
#include "Line.h"

#include <opencv2/core/core.hpp>

namespace Companion
{

    namespace Draw
    {

        /**
         * Implementation class to draw multiple lines.
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS Frame : public Drawable
        {

        public:

            /**
              * Constructor to create an frame.
              * @param topLeft Top left position.
              * @param topRight Top right position.
              * @param bottomLeft Bottom left position.
              * @param bottomRight Bottom right position.
              * @param color Color from frame.
              * @param thickness Thickness from frame.
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
            virtual ~Frame();

            /**
             * Draw lines to image.
             * @param image Image to draw lines.
             */
            virtual void draw(cv::Mat image);

            /**
             * Ratio to scale position from drawable to new location.
             * @param cWidth Current width.
             * @param cHeight Current height.
             * @param nWidth New width.
             * @param nHeight New height.
             */
            virtual void ratio(int cWidth, int cHeight, int nWidth, int nHeight);

            /**
             * Get top left point position from frame.
             * @return Top left point position.
             */
            const cv::Point &getTopLeft() const;

            /**
             * Get top right point position from frame.
             * @return Top right point position.
             */
            const cv::Point &getTopRight() const;

            /**
             * Get bottom left point position from frame.
             * @return Bottom left point position.
             */
            const cv::Point &getBottomLeft() const;

            /**
             * Get bottom right point position from frame.
             * @return Bottom right point position.
             */
            const cv::Point &getBottomRight() const;

            /**
             * Get color frame code.
             * @return Scalar color from frame.
             */
            const cv::Scalar &getColor() const;

            /**
             * Get thickness size from frame, if 0 no frame will be drawn.
             * @return Get thickness size from frame.
             */
            int getThickness() const;

            /**
             * Set top left position from frame.
             * @param topLeft Top left frame position.
             */
            void setTopLeft(const cv::Point &topLeft);

            /**
             * Set top right position from frame.
             * @param topRight Top right frame position.
             */
            void setTopRight(const cv::Point &topRight);

            /**
             * Set bottom left position from frame.
             * @param bottomLeft Bottom left frame position.
             */
            void setBottomLeft(const cv::Point &bottomLeft);

            /**
             * Set bottom right position from frame.
             * @param bottomRight Bottom right frame position.
             */
            void setBottomRight(const cv::Point &bottomRight);

            /**
             * Set color frame.
             * @param color Color frame to set.
             */
            void setColor(const cv::Scalar &color);

            /**
             * Set thickness size from frame.
             * @param thickness Thickness size.
             */
            void setThickness(int thickness);

        private:

            /**
             * Top left position from frame.
             */
            cv::Point topLeft;

            /**
             * Top right position from frame.
             */
            cv::Point topRight;

            /**
             * Bottom left position from frame.
             */
            cv::Point bottomLeft;

            /**
             * Bottom right position from frame.
             */
            cv::Point bottomRight;

            /**
             * Color from frame.
             */
            cv::Scalar color;

            /**
             * Thickness size from frame.
             */
            int thickness;
        };

    }
}

#endif //COMPANION_LINES_H
