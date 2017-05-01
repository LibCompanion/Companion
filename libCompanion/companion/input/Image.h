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

#ifndef COMPANION_IMAGE_H
#define COMPANION_IMAGE_H

#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "Stream.h"

namespace Companion {

    namespace Input {

        /**
         * Streaming class implementation for an subset from images.
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS Image : public Stream {

        public:

            /**
             * Default constructor to create an empty image list.
             */
            Image();

            /**
             * Default destructor;
             */
            ~Image();

            /**
             * Adds an given image path to images list.
             * @param path Image path to obtain image.
             */
            void addImagePath(std::string path);

            /**
             * Obtain next image from open video stream.
             * @return An empty cv::Mat object if no image is obtained otherwise an cv::Mat entity from image.
             */
            cv::Mat obtainImage();

        private:

            /**
             * List from all image paths;
             */
            std::vector<std::string> images;

            /**
             * Current index position from images.
             */
            int index;
        };

    }
}

#endif //COMPANION_IMAGE_H
