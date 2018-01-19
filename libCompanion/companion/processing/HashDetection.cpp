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

#include "HashDetection.h"

Companion::Processing::HashDetection::HashDetection(cv::Size modelSize, 
    Companion::Algorithm::Detection::ShapeDetection *shapeDetection,
    Companion::Algorithm::Hashing::Hashing *hashing)
{
	this->modelSize = modelSize;
	this->shapeDetection = shapeDetection;
    this->hashing = hashing;
	this->model = new Companion::Model::Processing::ImageHashModel();
}

Companion::Processing::HashDetection::~HashDetection()
{
	delete this->model;
}

bool Companion::Processing::HashDetection::addModel(int id, cv::Mat image)
{
	cv::Mat descriptor;

	// Resize images to correct size if needed
	if (image.cols != this->modelSize.width || image.rows != this->modelSize.height) 
	{
		Companion::Util::resizeImage(image, modelSize);
	}
	
	// detection converting to 1D Matrix and store to descriptor
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

CALLBACK_RESULT Companion::Processing::HashDetection::execute(cv::Mat frame) 
{
	cv::Mat query;
    CALLBACK_RESULT results;
    Companion::Model::Result* result;
    std::map<int, Companion::Model::Result*> scorings;

	// Obtain all shapes from image to detect.
	std::vector<Companion::Draw::Frame*> frames = shapeDetection->executeAlgorithm(frame);
    for (size_t i = 0; i < frames.size(); i++)
    {
        query = Util::cutImage(frame, frames.at(i)->cutArea());
        Companion::Util::resizeImage(query, modelSize);
        Companion::Util::convertColor(query, query, Companion::ColorFormat::GRAY);
        result = hashing->executeAlgorithm(this->model, query, frames.at(i));
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

    std::map<int, Companion::Model::Result*>::iterator it = scorings.begin();
    while (it != scorings.end())
    {
        // Obtain all results.
        results.push_back(it->second);
        // Increment the Iterator to point to next entry
        it++;
    }

    return results;
}

