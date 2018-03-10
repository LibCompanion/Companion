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

#ifndef COMPANION_VIDEO_H
#define COMPANION_VIDEO_H

#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include <companion/util/CompanionError.h>

#include "Stream.h"

namespace Companion { namespace Input
{
    /**
     * Video streaming OpenCV realization to obtain images from a video or a live stream.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS Video : public Stream
    {

    public:

        /**
         * Connects to a given physical video device.
         * @param device Device number to connect to.
         * @throws Companion::Error::Code if wrong device number is selected.
         */
        Video(int device);

        /**
         * Plays a video stream from the given URL.
         * @param url Path to video stream.
         * @throws Companion::Error::Code if invalid URL is used.
         */
        Video(std::string url);

        /**
         * Destructor.
         */
        virtual ~Video();

        /**
         * Obtain next image from open video stream.
         * @return An empty cv::Mat object if no image is obtained otherwise a cv::Mat entity from the obtained image.
         */
        cv::Mat obtainImage();

        /**
         * Indicator if stream has finished.
         * @return True if video has finished otherwise false.
         */
        bool isFinished();

        /**
         * Stop this video stream.
         */
        void finish();

    private:

        /**
         * Stores video device or video stream.
         */
        cv::VideoCapture capture;

        /**
         * Indicator if video has finished.
         */
        bool finished;
    };
}}

#endif //COMPANION_VIDEO_H
