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

#ifndef COMPANION_STREAM_H
#define COMPANION_STREAM_H

/**
 * Data stream interface class to obtain image processing data for example from an video or an set of images.
 * @author Andreas Sekulski
 */
class Stream {

public:

    /**
     * Obtain next image from open video stream.
     * @return An empty cv::Mat object if no image is obtained otherwise an cv::Mat entity from image.
     */
    virtual cv::Mat obtainImage() = 0;
};

#endif //COMPANION_STREAM_H