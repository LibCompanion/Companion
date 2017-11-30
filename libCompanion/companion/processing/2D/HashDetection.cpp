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

Companion::Processing::HashDetection::HashDetection(cv::Size modelSize, Companion::Algorithm::ShapeDetection *shapeDetection)
{
	this->modelSize = modelSize;
	this->shapeDetection = shapeDetection;
	this->model = new Companion::Model::Processing::ImageHashModel();
}

Companion::Processing::HashDetection::~HashDetection()
{
	delete model;
}

bool Companion::Processing::HashDetection::addModel(int id, cv::Mat image)
{
	// ToDo := Check if image is grayscale AND SAME SIZE if not abort --> false...

	cv::Mat descriptor;

	// Resize images to correct size if needed
	if (image.cols != this->modelSize.width || image.rows != this->modelSize.height) 
	{
		Companion::Util::resizeImage(image, modelSize);
	}
	
	// 2D converting to 1D Matrix and store to descriptor
	image.reshape(1, 1).convertTo(descriptor, CV_32F);

	// Store this id for a scoring
	scores.push_back({id, 0});
	
	// Store each 1D Matrix to dataset
	// 0 (...)
	// 1 (...)
	// 2 (...)
	// ...
	// n (n)
	model->addDescriptor(descriptor);
	
	return true;
}

CALLBACK_RESULT Companion::Processing::HashDetection::execute(cv::Mat frame) 
{

	cv::Mat query;
	std::pair<cv::Mat_<float>, cv::Mat> dataset = model->generateDataset();

	// Obtain all shapes from image to detect.
	std::vector<Companion::Draw::Frame*> frames = shapeDetection->executeAlgorithm(frame);

	// LSH implementation begin...
	// This code should be in a specific algorithm class... ToDo :-)
	cv::Mat_<float> hashImages = dataset.first;
	cv::Mat datasetImages = dataset.second;

	for (int i = 0; i < frames.size(); i++)
	{
		query = Util::cutImage(frame, frames.at(i)->cutArea());
		Companion::Util::resizeImage(query, modelSize);
		Companion::Util::convertColor(query, query, Companion::ColorFormat::GRAY);

		query.reshape(1, 1).convertTo(query, CV_32F);
		query = query * hashImages;

		for (int i = 0; i < query.rows; i++)
		{
			for (int j = 0; j < query.cols; j++)
			{
				query.at<float>(i, j) = query.at<float>(i, j) > 0 ? 1 : 0;
			}
		}

		query.convertTo(query, CV_8U);

		//Search for similar samples in the dataset
		for (int row = 0; row < datasetImages.rows; ++row)
		{
			scores[row].second = norm(query, datasetImages.row(row), cv::NORM_HAMMING);
		}

		//Make a copy of scores and rank them
		std::vector<std::pair<int, float>> rank;
		rank.assign(scores.begin(), scores.end());
		sort(rank.begin(), rank.end(), [](auto &a, auto &b)
		{
			return a.second < b.second;
		});

		// LSH implementation end result should be returned... ToDo

		// ToDo := SCORING VALUE
		for (int r = 0; r < std::min(10, (int)rank.size()); ++r)
		{
			std::cout << "RANK ID :" << rank.at(r).first << " Value " << rank.at(r).second << std::endl;
		}
		std::cout << "-------------------------------------------------------"<< std::endl;
	}

    return std::vector<Companion::Model::Result *>();
}


