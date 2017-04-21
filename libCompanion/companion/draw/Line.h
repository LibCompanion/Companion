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

#ifndef COMPANION_LINE_H
#define COMPANION_LINE_H

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "Drawable.h"
#include "companion/util/Util.h"
#include "companion/util/exportapi/ExportAPIDefinitions.h"

namespace Companion {

    namespace Draw {

        /**
         * Class implementation to draw an single line.
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS Line : public Drawable {

        public:

            /**
             * Constructor to create an single line.
             * @param start Start draw position from line.
             * @param end End draw position from line
             * @param color Color from line.
             * @param thickness Thickness from line.
             */
            Line(cv::Point start, cv::Point end, cv::Scalar color, int thickness);

            /**
             * Get starting point from line to draw.
             * @return Starting point from line.
             */
            const cv::Point &getStart() const;

            /**
             * Sets an given starting point from line.
             * @param start Starting point to set from line.
             */
            void setStart(const cv::Point &start);

            /**
             * Get endpoint from line.
             * @return Endpoint from line.
             */
            const cv::Point &getEnd() const;

            /**
             * Sets given endpoint from line.
             * @param end Endpoint to set.
             */
            void setEnd(const cv::Point &end);

            /**
             * Get color from line.
             * @return Color code from line.
             */
            const cv::Scalar &getColor() const;

            /**
             * Sets given color code for line.
             * @param color Color to draw line.
             */
            void setColor(const cv::Scalar &color);

            /**
             * Get thickness from line.
             * @return Line thicknees.
             */
            int getThickness() const;

            /**
             * Sets given line thickness must be greater or equal 1.
             * @param thickness Thickness to set.
             */
            void setThickness(int thickness);

            /**
             * Draw an single line to image.
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

        private:

            /**
             * Starting point from line.
             */
            cv::Point start;

            /**
             * Endpoint from line.
             */
            cv::Point end;

            /**
             * Color code from line.
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