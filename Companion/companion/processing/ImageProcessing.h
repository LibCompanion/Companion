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

#ifndef COMPANION_IMAGEPROCESSING_H
#define COMPANION_IMAGEPROCESSING_H

#include <opencv2/core/core.hpp>
#include <companion/util/Definitions.h>

namespace Companion { namespace Processing
{
    /**
     * Image processing interface class to create specific image processing jobs for example to recognize or detect objects.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS ImageProcessing
    {

    public:

        /**
         * Execute given image processing algorithm like object detection or object recognition.
         * @param frame Source image for the image processing.
         * @return A vector of results if there are any.
         */
        virtual CALLBACK_RESULT execute(cv::Mat frame) = 0;
    };
}}

#endif //COMPANION_IMAGEPROCESSING_H
