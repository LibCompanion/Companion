/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2018 Andreas Sekulski
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

namespace Companion { namespace Algorithm { namespace Detection
{
    /**
     * Possible shapes.
     */
    enum class COMP_EXPORTS Shape
    {
        QUAD
    };

    /**
     * Shape detection implementation to detect specifially shaped regions of interest.
     * @author Andreas Sekulski
     */
    class COMP_EXPORTS ShapeDetection : public Detection
    {

    public:

        /**
         * Shape detection construtor. Shape detection functions are used in this order: dilate(erode(morph(image))).
         * @param morphKernel Morphology kernel size.
         * @param erodeKernel Erode kernel size.
         * @param dilateKernel Dilate kernel size.
         * @param shape Shape type (default is QUAD).
         * @param cannyThreshold Canny threshold to indicate corners.
         * @param dilateIteration Count dilate iterations.
         */
        ShapeDetection(cv::Mat morphKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 30)),
                       cv::Mat erodeKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10)),
                       cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(40, 40)),
                       Shape shape = Shape::QUAD,
                       double cannyThreshold = 50.0,
                       int dilateIteration = 3);

        /**
         * Default destructor.
         */
        virtual ~ShapeDetection();

        /**
         * Shape detection algorithm to obtain possible regions of interest (ROI).
         * @param frame Image frame to obtain all roi objects from.
         * @throws Companion::Error::Code If an error occurred in search operation.
         * @return A vector of frames that represent the detected shapes.
         */
        std::vector<Companion::Draw::Frame*> executeAlgorithm(cv::Mat frame);

        /**
         * Indicator if this algorithm uses cuda.
         * @return True if cuda will be used otherwise false for CPU/OpenCL usage.
         */
        bool isCuda() const;

        /**
         * Return the detection shape.
         * @return Detection shape.
         */
        Shape getShape() const;

    private:

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
         * Canny threshold value to obtain edges.
         */
        double cannyThreshold;

        /**
         * Shape type.
         */
        Shape shape;

        /**
         * Number of dilate iterations.
         */
        int dilateIteration;

        /**
         * Quad detection algorithm to obtain possible regions of interest (ROI).
         * @param frame Image frame to obtain all roi objects from.
         * @throws Companion::Error::Code If an error occurred in search operation.
         * @return A vector of frames that represent the detected quads.
         */
        std::vector<Companion::Draw::Frame*> obtainQuads(cv::Mat frame);
    };
}}}

#endif //COMPANION_SHAPEDETECTION_H
