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
#include <queue>
#include <mutex>
#include <condition_variable>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "Stream.h"

namespace Companion
{

	namespace Input
	{

		/**
		 * Streaming class implementation for an subset from images.
		 * @author Andreas Sekulski
		 */
		class COMP_EXPORTS Image : public Stream
		{

		public:

			/**
			 * Default constructor to create an empty image list.
			 * @param maxImages Maximum amount of images that can be loaded at the same time.
			 */
			Image(int maxImages);

			/**
			 * Default destructor;
			 */
			~Image();

			/**
			 * Store an image to FIFO.
			 * @param imgPath Image path to store.
			 * @return <code>true</code> if image is stored otherwise <code>false</code> if image not exists.
			 */
			bool addImage(std::string imgPath);

			/**
			 * Stores an given image FIFO.
			 * @param img Image to store.
			 * @return <code>true</code> if image is stored otherwise <code>false</code> if image not exists.
			 */
			bool addImage(cv::Mat img);

			/**
			 * Stores an given image FIFO.
			 * @param width Width of the image to store.
			 * @param height Height of the image to store.
			 * @param type Type of the image to store.
			 * @param data Raw image data to store.
			 * @return <code>true</code> if image is stored otherwise <code>false</code>.
			 */
			bool addImage(int width, int height, int type, uchar* data);

			/**
			 * Obtain next image from open video stream.
			 * @return An empty cv::Mat object if no image is obtained otherwise an cv::Mat entity from image.
			 */
			cv::Mat obtainImage();

			/**
			 * Indicator if stream is finished.
			 * @return <code>true</code> if image stream is finished, <code>false</code> otherwise
			 */
			bool isFinished();

			/**
			 * Method to signal to stop image stream.
			 */
			void finish();

			/**
			 * Method to signal to stop image stream after all images were processed.
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
			 * List from all stored images as an fifo;
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

	}
}

#endif //COMPANION_IMAGE_H
