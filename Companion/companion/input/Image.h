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

#ifndef COMPANION_IMAGE_H
#define COMPANION_IMAGE_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "Stream.h"

namespace Companion { namespace Input
{
    /**
     * Streaming class implementation for an image stream.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS Image : public Stream
    {

    public:

        /**
         * Default constructor to create an empty image stream.
         * @param maxImages Maximum amount of images that can be loaded at the same time.
         */
        Image(int maxImages);

        /**
         * Destructor;
         */
        virtual ~Image();

        /**
         * Store image from given path to FIFO.
         * @param imgPath Image path to store image from.
         * @return <code>True</code> if the image was stored and <code>false</code> if image not exists.
         */
        bool addImage(std::string imgPath);

        /**
         * Store a given image to FIFO.
         * @param img Image to store.
         * @return <code>True</code> if the image was stored and <code>false</code> if image not exists.
         */
        bool addImage(cv::Mat img);

        /**
         * Store a given image to FIFO.
         * @param width Width of the image to store.
         * @param height Height of the image to store.
         * @param type Type of the image to store.
         * @param data Raw image data to store.
         * @return <code>True</code> if the image was stored and <code>false</code> if image not exists.
         */
        bool addImage(int width, int height, int type, uchar* data);

        /**
         * Obtain next image from open image stream.
         * @return An empty cv::Mat object if no image is obtained otherwise an cv::Mat entity from the obtained image.
         */
        cv::Mat obtainImage();

        /**
         * Indicator if stream has finished.
         * @return <code>True</code> if image stream has finished, <code>false</code> otherwise.
         */
        bool isFinished();

        /**
         * Stop this stream.
         */
        void finish();

        /**
         * Stop image stream after all images were processed.
         */
        void finishAfterProcessing();

    private:

        /**
         * Indicator to stop this stream.
         */
        bool exitStream;

        /**
         * Indicator to stop this stream after all images were processed.
         */
        bool exitAfterProcessing;

        /**
         * List of all stored images as FIFO;
         */
        std::queue<cv::Mat> images;

        /**
         * Mutex to control the image stream to keep memory low.
         */
        std::mutex mx;

        /**
         * Waiting condition for image input stream;
         */
        std::condition_variable cv;

        /**
         * Maximum amount of images that can be loaded at the same time.
         */
        int maxImages;
    };
}}

#endif //COMPANION_IMAGE_H
