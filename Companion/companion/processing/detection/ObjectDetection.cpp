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

#include "ObjectDetection.h"

Companion::Processing::Detection::ObjectDetection::ObjectDetection(PTR_SHAPE_DETECTION detection)
{
    this->detection = detection;
}

CALLBACK_RESULT Companion::Processing::Detection::ObjectDetection::Execute(cv::Mat frame)
{
	CALLBACK_RESULT results;

    // Detection algorithm is shape detection
    // Obtain all detectable objects from the given image
    std::vector<PTR_DRAW_FRAME> frames = this->detection->ExecuteAlgorithm(frame);
    for (size_t i = 0; i < frames.size(); i++)
    {
		PTR_RESULT_DETECTION result = std::make_shared<RESULT_DETECTION>(100, detection->Description(), frames[i]);
        results.push_back(result);
    }

    return results;
}
