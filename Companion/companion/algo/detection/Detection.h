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

#ifndef COMPANION_DETECTION_H
#define COMPANION_DETECTION_H

#include <companion/draw/Frame.h>

namespace Companion { namespace Algorithm { namespace Detection
{
    /**
     * Detection abstract class to detect specific regions of interest.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS Detection
    {

    public:

        /**
         * Detection algorithm to detect specific regions of interest (ROI).
         * @param frame Image frame to obtain all roi objects from.
         * @return A vector of frames that represent the detected regions.
         */
        virtual std::vector<Companion::Draw::Frame*> executeAlgorithm(cv::Mat frame) = 0;

        /**
         * Indicator if this algorithm uses cuda.
         * @return True if cuda will be used otherwise false for CPU/OpenCL usage.
         */
        virtual bool isCuda() const = 0;
    };
}}}

#endif //COMPANION_DETECTION_H
