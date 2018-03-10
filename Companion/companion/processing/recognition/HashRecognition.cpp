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
    Companion::Algorithm::Detection::ShapeDetection *shapeDetection,
    Companion::Algorithm::Recognition::Hashing::Hashing *hashing)
{
	this->modelSize = modelSize;
	this->shapeDetection = shapeDetection;
    this->hashing = hashing;
	this->model = new Companion::Model::Processing::ImageHashModel();
}

Companion::Processing::Recognition::HashRecognition::~HashRecognition()
{
	delete this->model;
}

bool Companion::Processing::Recognition::HashRecognition::addModel(int id, cv::Mat image)
{
	cv::Mat descriptor;

	// Resize images to correct size if needed
	if (image.cols != this->modelSize.width || image.rows != this->modelSize.height) 
	{
		Companion::Util::resizeImage(image, this->modelSize);
	}
	
	// Convert image to 1D Matrix and store to descriptor
	image.reshape(1, 1).convertTo(descriptor, CV_32F);

	// Store each 1D Matrix to dataset
	// 0 (...)
	// 1 (...)
	// 2 (...)
	// ...
	// n (n)
	this->model->addDescriptor(id, descriptor);
	
	return true;
}

CALLBACK_RESULT Companion::Processing::Recognition::HashRecognition::execute(cv::Mat frame)
{
	cv::Mat query;
    CALLBACK_RESULT results;
    Companion::Model::Result::RecognitionResult* result;
    std::map<int, Companion::Model::Result::RecognitionResult*> scorings;

	// Obtain all shapes from image to recognize
	std::vector<Companion::Draw::Frame*> frames = this->shapeDetection->executeAlgorithm(frame);
    for (size_t i = 0; i < frames.size(); i++)
    {
        query = Util::cutImage(frame, frames.at(i)->cutArea());
        Companion::Util::resizeImage(query, this->modelSize);
        Companion::Util::convertColor(query, query, Companion::ColorFormat::GRAY);
        result = this->hashing->executeAlgorithm(this->model, query, frames.at(i));
        if (result != nullptr)
        {
            // Score only best results from rois
            if (scorings.find(result->getId()) == scorings.end()) {
                scorings[result->getId()] = result;
            } else if(scorings[result->getId()]->getScoring() < result->getScoring()) {
                scorings[result->getId()] = result;
            }
        }
    }

    std::map<int, Companion::Model::Result::RecognitionResult*>::iterator it = scorings.begin();
    while (it != scorings.end())
    {
        // Obtain all results.
        results.push_back(it->second);
        // Increment the Iterator to point to next entry
        it++;
    }

    return results;
}
