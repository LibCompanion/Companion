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

#ifndef COMPANION_IRA_H
#define COMPANION_IRA_H

#include <opencv2/core/core.hpp>
#include <companion/util/exportapi/ExportAPIDefinitions.h>

namespace Companion { namespace Algorithm { namespace Matching { namespace UTIL
{
    /**
     * Image reduction algorithm (IRA) implementation to improve performance for an supported image recognition.
     * @author Andreas Sekulski
     */
    class COMP_EXPORTS IRA
    {

    public:

        /**
         * Default constructor to create an empty IRA object.
         */
        IRA();

        /**
         * Default destructor.
         */
        virtual ~IRA();

        /**
         * Gets last object detection position if exists.
         * @return Gets last object detection position if exists, otherwise position is -1.
         */
        const cv::Rect &getLastObjectPosition() const;

        /**
         * Sets last object position from detection.
         * @param x X-Axis position from object.
         * @param y Y-Axis position from object.
         * @param width Width from object.
         * @param height Height from object.
         */
        void setLastObjectPosition(int x, int y, int width, int height);

        /**
         * Sets width from detected object.
         * @param width Width from homography object.
         */
        void setWidth(int width);

        /**
         * Sets height from detected object.
         * @param height Height from homography object.
         */
        void setHeight(int height);

        /**
         * Sets x position from object.
         * @param x X position from homography object.
         */
        void setX(int x);

        /**
         * Sets y position from object.
         * @param y Y position from homography object.
         */
        void setY(int y);

        /**
         * Clears last object detection position.
         */
        void clear();

        /**
         * Checks if last position from object exists.
         * @return <b>true<br> if last position is set otherwise <b>false</b>
         */
        bool isObjectDetected();

    private:

        /**
         * Last object position (lop) from detected object from last scene image.
         */
        cv::Rect lop;

        /**
         * Default initial position if no object was detected in last frame.
         */
        const int NO_OBJECT_DETECTED = 0;

    };
}}}}

#endif //COMPANION_IRA_H
