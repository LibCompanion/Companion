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

#include "HashRecognition.h"

Companion::Processing::Recognition::HashRecognition::HashRecognition(cv::Size modelSize,
	PTR_SHAPE_DETECTION shapeDetection,
	PTR_HASHING hashing)
{
    this->modelSize = modelSize;
    this->shapeDetection = shapeDetection;
    this->hashing = hashing;
    this->model = std::make_shared<MODEL_IMAGE_HASHING>();
}

bool Companion::Processing::Recognition::HashRecognition::AddModel(int id, cv::Mat image)
{
    cv::Mat descriptor;

    // Resize images to correct size if needed
    if (image.cols != this->modelSize.width || image.rows != this->modelSize.height) 
    {
        Companion::Util::ResizeImage(image, this->modelSize);
    }
    
    // Convert image to 1D Matrix and store to descriptor
    image.reshape(1, 1).convertTo(descriptor, CV_32F);

    // Store each 1D Matrix to dataset
    // 0 (...)
    // 1 (...)
    // 2 (...)
    // ...
    // n (n)
    this->model->AddDescriptor(id, descriptor);
    
    return true;
}

CALLBACK_RESULT Companion::Processing::Recognition::HashRecognition::Execute(cv::Mat frame)
{
    cv::Mat query;
    CALLBACK_RESULT results;
	PTR_RESULT_RECOGNITION result;
    std::map<int, PTR_RESULT_RECOGNITION> scorings;

    // Obtain all shapes from the image to recognize
    std::vector<PTR_DRAW_FRAME> frames = this->shapeDetection->ExecuteAlgorithm(frame);
    for (size_t i = 0; i < frames.size(); i++)
    {
        query = Util::CutImage(frame, frames.at(i)->CutArea());
        Companion::Util::ResizeImage(query, this->modelSize);
        Companion::Util::ConvertColor(query, query, Companion::ColorFormat::GRAY);
        result = this->hashing->ExecuteAlgorithm(this->model, query, frames.at(i));
        if (result != nullptr)
        {
            // Score only best results from ROIs
            if (scorings.find(result->Id()) == scorings.end()) {
                scorings[result->Id()] = result;
            } else if(scorings[result->Id()]->Scoring() < result->Scoring()) {
                scorings[result->Id()] = result;
            }
        }
    }

    std::map<int, PTR_RESULT_RECOGNITION>::iterator it = scorings.begin();
    while (it != scorings.end())
    {
        // Obtain all results
        results.push_back(it->second);
        // Increment the Iterator to point to next entry
        it++;
    }

    return results;
}
