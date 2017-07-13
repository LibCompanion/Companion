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

namespace Companion {

    namespace Draw {

        /**
         * Implementation class to draw multiple lines.
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS Frame : public Drawable {

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
                  cv::Scalar color = cv::Scalar(0, 255, 0, 255),
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

            const cv::Point &getTopLeft() const;

            const cv::Point &getTopRight() const;

            const cv::Point &getBottomLeft() const;

            const cv::Point &getBottomRight() const;

            const cv::Scalar &getColor() const;

            int getThickness() const;

            void setTopLeft(const cv::Point &topLeft);

            void setTopRight(const cv::Point &topRight);

            void setBottomLeft(const cv::Point &bottomLeft);

            void setBottomRight(const cv::Point &bottomRight);

            void setColor(const cv::Scalar &color);

            void setThickness(int thickness);

        private:

            cv::Point topLeft;

            cv::Point topRight;

            cv::Point bottomLeft;

            cv::Point bottomRight;

            cv::Scalar color;

            int thickness;
        };

    }
}

#endif //COMPANION_LINES_H