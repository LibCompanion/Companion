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

#ifndef COMPANION_PRODUCERSTREAM_H
#define COMPANION_PRODUCERSTREAM_H

#include <queue>
#include <opencv2/core.hpp>
#include <companion/openCV3/stream/Video.h>
#include <companion/openCV3/util/CompanionError.h>
#include <companion/openCV3/util/Util.h>
#include <boost/lockfree/spsc_queue.hpp>

/**
 * Producer stream class implementation to generate images from an video source.
 * @author Andreas Sekulski
 */
class ProducerStream {

public:

    /**
     * Constructor to create an producer streaming class to obtain images from an video and store to an queue.
     * @param queue Queue to store obtained images.
     */
    ProducerStream(boost::lockfree::spsc_queue<cv::Mat> &queue) : queue(queue) {}

    /**
     * Run method to start an video stream and store an queue.
     * @param videoPath Video to load and store images to his queue.
     */
    void run(std::string videoPath);

    // ToDo := Obtain livestream images.

private:

    /**
     * Queue to store images from video stream.
     */
    boost::lockfree::spsc_queue<cv::Mat> &queue;
};

#endif //COMPANION_PRODUCERSTREAM_H