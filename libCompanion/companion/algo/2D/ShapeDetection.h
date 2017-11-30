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

#ifndef COMPANION_SHAPEDETECTION_H
#define COMPANION_SHAPEDETECTION_H

#include <companion/algo/abstract/ImageRecognition.h>
#include <companion/model/result/Result.h>
#include <companion/draw/Frame.h>
#include <companion/util/CompanionError.h>
#include <opencv2/imgproc.hpp>

namespace Companion
{

	namespace Algorithm
	{

		/**
		 * Shape detection implementation to detect region of interest.
		 * @author Andreas Sekulski
		 */
		class COMP_EXPORTS ShapeDetection : public ImageRecognition
		{

		public:

			/**
			 * Shape detection construtor to detect different shapes. Shape detection function used in this order
			 * dilate(erode(morph(image)))
			 * @param morphKernel Morphology kernel size.
			 * @param erodeKernel Erode kernel size.
			 * @param dilateKernel Dilate kernel size.
			 * @param cannyThreshold Canny threshold to indicate corners.
			 * @param dilateIteration Count dilate iterations.
			 */
			ShapeDetection(cv::Mat morphKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 30)),
				cv::Mat erodeKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10)),
				cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(40, 40)),
				int cannyThreshold = 50,
				int dilateIteration = 3);

			virtual ~ShapeDetection();

			/**
			 * Shape detection algorithm detection to obtain possible region of interest (ROI).
			 * @param frame Image frame to obtain all roi objects.
			 * @throws Companion::Error::Code If an error occurred in search operation.
			 * @return An result model if an object is detected otherwise nullptr.
			 */
			std::vector<Companion::Draw::Frame*> executeAlgorithm(cv::Mat frame);

			/**
			 * Indicator if this algorithm use cuda.
			 * @return True if cuda will be used otherwise false for CPU/OpenCL usage.
			 */
			bool isCuda();

		private:

			/**
			 * Canny threshold value to obtain edges.
			 */
			int cannyThreshold;

			/**
			 * Morphology transformation kernel for morphologyEx operation.
			 */
			cv::Mat morphKernel;

			/**
			 * Erode tranformation kernel for erode operation.
			 */
			cv::Mat erodeKernel;

			/**
			 * Dilate kernel for dilate operation.
			 */
			cv::Mat dilateKernel;

			/**
			 * Dilate iteration counter to repeat dilate operation.
			 */
			int dilateIteration;
		};

	}
}



#endif //COMPANION_SHAPEDETECTION_H
