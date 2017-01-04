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

#ifndef COMPANION_CONSUMERSTREAM_H
#define COMPANION_CONSUMERSTREAM_H

#include <queue>
#include <opencv2/core.hpp>
#include <companion/openCV3/search/ImageRecognition.h>
#include <companion/openCV3/search/FeatureMatching.h>
#include <boost/lockfree/spsc_queue.hpp>

/**
 * Consumer class implementation for an video stream.
 * @author Andreas Sekulski
 */
class ConsumerStream {

public:

    /**
     * Constructor to create an consumer stream class.
     * @param queue Queue to store all images.
     */
    ConsumerStream(boost::lockfree::spsc_queue<cv::Mat>& queue) : queue(queue) {}

    /**
     * Run method for an thread operation.
     * @param imgPath Image to compare with produced images.
     */
    void run(std::string imgPath);

private:

    /**
     * Boost lockfree queue to obtain stored images from queue.
     */
    boost::lockfree::spsc_queue<cv::Mat> &queue;

};

#endif //COMPANION_CONSUMERSTREAM_H