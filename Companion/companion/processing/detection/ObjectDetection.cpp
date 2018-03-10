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

Companion::Processing::Detection::ObjectDetection::ObjectDetection(Companion::Algorithm::Detection::Detection *detection)
{
    this->detection = detection;
}

Companion::Processing::Detection::ObjectDetection::~ObjectDetection()
{
}

CALLBACK_RESULT Companion::Processing::Detection::ObjectDetection::execute(cv::Mat frame)
{
    Companion::Algorithm::Detection::ShapeDetection *shapeDetection;
    CALLBACK_RESULT results;

    shapeDetection = dynamic_cast<Companion::Algorithm::Detection::ShapeDetection*>(this->detection);
    if (shapeDetection != nullptr)
    {
        // Detection algorithm is shape detection
        // Obtain all detectable objects from the given image
        std::vector<Companion::Draw::Frame*> frames = this->detection->executeAlgorithm(frame);
        for (size_t i = 0; i < frames.size(); i++)
        {
            std::string description = "";
            switch (shapeDetection->getShape())
            {
                case Companion::Algorithm::Detection::Shape::QUAD:
                    description = "Quad";
                    break;
            }
            results.push_back(new Companion::Model::Result::DetectionResult(100, description, frames[i]));
        }
    }

    return results;
}
