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

#ifndef COMPANION_SHAPEDETECTION_H
#define COMPANION_SHAPEDETECTION_H

#include <opencv2/imgproc.hpp>
#include <companion/algo/detection/Detection.h>
#include <companion/util/CompanionError.h>

namespace Companion {
	namespace Algorithm {
		namespace Detection
		{
			/**
			 * Shape detection implementation to detect specifically shaped regions of interest.
			 * @author Andreas Sekulski, Dimitri Kotlovsky
			 */
			class COMP_EXPORTS ShapeDetection : public Detection
			{

			public:

				/**
				 * Shape detection constructor. Shape detection functions are used in this order: dilate(erode(morph(image))).
				 * @param minCorners Minimum number of shape corners.
				 * @param maxCorners Maximum number of shape corners.
				 * @param shapeDescription Shape description.
				 * @param cannyThreshold Canny threshold to indicate corners.
				 * @param dilateIteration Count dilate iterations.
				 * @param morphKernel Morphology kernel size.
				 * @param erodeKernel Erode kernel size.
				 * @param dilateKernel Dilate kernel size.
				 */
				ShapeDetection(int minCorners = 4,
					int maxCorners = 20,
					std::string shapeDescription = "Polygon",
					double cannyThreshold = 50.0,
					int dilateIteration = 3,
					cv::Mat morphKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 30)),
					cv::Mat erodeKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10)),
					cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(40, 40)));

				/**
				 * Destructor.
				 */
				virtual ~ShapeDetection() = default;

				/**
				 * Shape detection algorithm to obtain possible regions of interest (ROI).
				 * @param frame Image frame to obtain all roi objects from.
				 * @throws Companion::Error::Code If an error occurred in search operation.
				 * @return A vector of frames that represent the detected shapes.
				 */
				std::vector<PTR_DRAW_FRAME> ExecuteAlgorithm(cv::Mat frame);

				/**
				 * Indicator if this algorithm uses cuda.
				 * @return True if cuda will be used otherwise false for CPU/OpenCL usage.
				 */
				bool IsCuda() const;

				/**
				 * Get shape description.
				 */
				std::string Description() const;

			private:

				/**
				 * Morphology transformation kernel for morphologyEx operation.
				 */
				cv::Mat morphKernel;

				/**
				 * Erode transformation kernel for erode operation.
				 */
				cv::Mat erodeKernel;

				/**
				 * Dilate kernel for dilate operation.
				 */
				cv::Mat dilateKernel;

				/**
				 * Canny threshold value to obtain edges.
				 */
				double cannyThreshold;

				/**
				 * Minimum number of shape corners.
				 */
				int minCorners;

				/**
				 * Maximum number of shape corners.
				 */
				int maxCorners;

				/**
				 * Shape description.
				 */
				std::string shapeDescription;

				/**
				 * Number of dilate iterations.
				 */
				int dilateIteration;
			};
		}
	}
}

#endif //COMPANION_SHAPEDETECTION_H
