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

#include "ShapeDetection.h"

Companion::Algorithm::Detection::ShapeDetection::ShapeDetection(cv::Mat morphKernel, cv::Mat erodeKernel, cv::Mat dilateKernel, double cannyThreshold, int dilateIteration)
{
    this->morphKernel = morphKernel;
    this->erodeKernel = erodeKernel;
    this->dilateKernel = dilateKernel;
    this->cannyThreshold = cannyThreshold;
    this->dilateIteration = dilateIteration;
}

Companion::Algorithm::Detection::ShapeDetection::~ShapeDetection()
{

}

std::vector<Companion::Draw::Frame*> Companion::Algorithm::Detection::ShapeDetection::executeAlgorithm(cv::Mat frame)
{
	cv::Point topLeft; // Top left coordinate from rectangl.e
	cv::Point bottomRight; // Bottom right coordinate from rectangle
	cv::Point diff;
	int minDistance;
	int mostLeftCoordinate;
	int mostRightCoordinate;
	int heighestCoordinate;
	int lowestCoordinate;

	int width; // Width from rectangle
	int height; // Height from rectangle

	cv::Mat result = cv::Mat::zeros(frame.size(), CV_8UC3);
	std::vector<Companion::Draw::Frame*> rois;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::Point> approx;

	if (frame.empty())
	{
		throw Companion::Error::Code::image_not_found;
	}

	cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    cv::Canny(frame, frame, this->cannyThreshold, this->cannyThreshold * 3.0, 3);

	// Morphological Transformations - http://docs.opencv.org/trunk/d9/d61/tutorial_py_morphological_ops.html
    morphologyEx(frame, frame, CV_MOP_CLOSE, this->morphKernel);
    erode(frame, frame, this->erodeKernel);
    dilate(frame, frame, this->dilateKernel, cv::Point(-1, -1), this->dilateIteration);

	// Contour Retrieval Mode - http://docs.opencv.org/3.1.0/d9/d8b/tutorial_py_contours_hierarchy.html
	// CV_RETR_EXTERNAL, CV_RETR_LIST, CV_RETR_CCOMP, CV_RETR_TREE
	findContours(frame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	for (int i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.01, true);

		// Default values
		mostLeftCoordinate = result.size().width;
		lowestCoordinate = result.size().height;
		mostRightCoordinate = 0;
		heighestCoordinate = 0;

		if ((approx.size() >= 4) && (approx.size() <= 20))
		{
			for (int i = 0; i < approx.size(); i++)
			{
				mostLeftCoordinate = (approx.at(i).x < mostLeftCoordinate) ? approx.at(i).x : mostLeftCoordinate;
				lowestCoordinate = (approx.at(i).y < lowestCoordinate) ? approx.at(i).y : lowestCoordinate;
				mostRightCoordinate = (approx.at(i).x > mostRightCoordinate) ? approx.at(i).x : mostRightCoordinate;
				heighestCoordinate = (approx.at(i).y > heighestCoordinate) ? approx.at(i).y : heighestCoordinate;
			}

			topLeft = cv::Point(mostLeftCoordinate, lowestCoordinate);
			bottomRight = cv::Point(mostRightCoordinate, heighestCoordinate);
			diff = bottomRight - topLeft;
			minDistance = result.size().width / 4.0f;

			if (sqrt(diff.x * diff.x + diff.y * diff.y) > minDistance)
			{
				width = bottomRight.x - topLeft.x;
				height = bottomRight.y - topLeft.y;

				rois.push_back(new Companion::Draw::Frame(topLeft,
					cv::Point(topLeft.x + width, topLeft.y),
					cv::Point(topLeft.x, topLeft.y + height),
					bottomRight));
			}
		}
	}

	return rois;
}

bool Companion::Algorithm::Detection::ShapeDetection::isCuda()
{
	return false;
}
