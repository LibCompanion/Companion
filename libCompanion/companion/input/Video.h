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

#ifndef COMPANION_VIDEO_CAPTURE_H
#define COMPANION_VIDEO_CAPTURE_H

#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include <companion/util/CompanionError.h>

#include "Stream.h"

namespace Companion { namespace Input
{
    /**
     * Video streaming OpenCV realization to obtain images from an video or livestream.
     * @author Andreas Sekulski
     */
    class COMP_EXPORTS Video : public Stream
    {

    public:

        /**
          * Connects to an given physical device.
          * @param device Device number to connect.
          * @throws Companion::Error::Code If wrong device number is selected.
          */
        Video(int device);

        /**
         * Plays an video stream from given url.
         * @param url Video path to stream.
         * @throws Companion::Error::Code If invalid url is used.
         */
        Video(std::string url);

        /**
         * Default destructor.
         */
        virtual ~Video();

        /**
         * Obtain next image from open video stream.
         * @return An empty cv::Mat object if no image is obtained otherwise an cv::Mat entity from image.
         */
        cv::Mat obtainImage();

        /**
         * Indicator if stream is finished.
         * @return True if video is finished otherwise false.
         */
        bool isFinished();

        /**
         * Stop this stream.
         */
        void finish();

    private:

        /**
         * Stores video device or video streaming.
         */
        cv::VideoCapture capture;

        /**
         * Indicator if video is finished.
         */
        bool finished;
    };
}}

#endif //COMPANION_VIDEO_CAPTURE_H
