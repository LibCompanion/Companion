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

#ifndef COMPANION_DRAWABLE_H
#define COMPANION_DRAWABLE_H

#include <opencv2/core/core.hpp>

namespace Companion {

    namespace Draw {

        /**
         * Abstract class implementation for an area.
         * @author Andreas Sekulski.
         */
        class Drawable {

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
        };
    }
}

#endif //COMPANION_DRAWABLE_H