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

#include "ShapeDetection.h"

Companion::Algorithm::Detection::ShapeDetection::ShapeDetection(
    int minCorners,
    int maxCorners,
    std::string shapeDescription,
    double cannyThreshold,
    int dilateIteration,
    cv::Mat morphKernel,
    cv::Mat erodeKernel,
    cv::Mat dilateKernel)
{
    this->minCorners = minCorners;
    this->maxCorners = maxCorners;
    this->shapeDescription = shapeDescription;
    this->cannyThreshold = cannyThreshold;
    this->dilateIteration = dilateIteration;
    this->morphKernel = morphKernel;
    this->erodeKernel = erodeKernel;
    this->dilateKernel = dilateKernel;
}

Companion::Algorithm::Detection::ShapeDetection::~ShapeDetection()
{
}

std::vector<Companion::Draw::Frame*> Companion::Algorithm::Detection::ShapeDetection::executeAlgorithm(cv::Mat frame)
{
    std::vector<Companion::Draw::Frame*> rois;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Point> approx;
    int minDistance = frame.size().width / 4.0f;

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

    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.01, true);
        cv::Rect rect = cv::boundingRect(approx);

        // Check number of corners (vertices)
        if ((approx.size() >= this->minCorners) && (approx.size() <= this->maxCorners))
        {
            // Check size
            double diagonale = sqrt((rect.width * rect.width) + (rect.height * rect.height));
            if (diagonale > minDistance)
            {
                rois.push_back(new Companion::Draw::Frame(
                    cv::Point(rect.x, rect.y),
                    cv::Point(rect.x + rect.width, rect.y),
                    cv::Point(rect.x, rect.y + rect.height),
                    cv::Point(rect.x + rect.width, rect.y + rect.height)
                ));
            }
        }
    }

    return rois;
}

bool Companion::Algorithm::Detection::ShapeDetection::isCuda() const
{
    return false;
}

std::string Companion::Algorithm::Detection::ShapeDetection::getDescription() const
{
    return this->shapeDescription;
}
