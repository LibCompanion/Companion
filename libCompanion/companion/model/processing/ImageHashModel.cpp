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

#include "ImageHashModel.h"

Companion::Model::Processing::ImageHashModel::ImageHashModel()
{
	this->newModelAdded = true;
}

Companion::Model::Processing::ImageHashModel::~ImageHashModel()
{
}

void Companion::Model::Processing::ImageHashModel::addDescriptor(int id, cv::Mat &descriptor)
{
	imageDataset.push_back(descriptor);
    
    // Store this id for a scoring
    scores.push_back({id, 0});
    
    this->newModelAdded = true;
}

std::pair<cv::Mat_<float>, cv::Mat> Companion::Model::Processing::ImageHashModel::generateDataset()
{
    std::pair<cv::Mat_<float>, cv::Mat> dataset;
    dataset.first = generateHashImages();
    dataset.second = generateIndexDataset(dataset.first);
	this->newModelAdded = false;
    return dataset;
}

cv::Mat_<float> Companion::Model::Processing::ImageHashModel::generateHashImages()
{
	if (newModelAdded)
	{
		cv::Mat_<float> hash(imageDataset.cols, 100);
		std::default_random_engine gen;
		std::normal_distribution<float> dist(0, 1);

		for (int i = 0; i < hash.rows; i++)
		{
			for (int j = 0; j < hash.cols; j++)
			{
				hash.at<float>(i, j) = dist(gen);
			}
		}

		this->hash = hash;
	}
	
	return this->hash;
}

cv::Mat Companion::Model::Processing::ImageHashModel::generateIndexDataset(cv::Mat_<float> hash)
{
	if (newModelAdded)
	{
		cv::Mat images = imageDataset * hash;

		for (size_t i = 0; i < images.rows; i++)
		{
			for (size_t j = 0; j < images.cols; j++)
			{
				images.at<float>(i, j) = images.at<float>(i, j) > 0 ? 1 : 0;
			}
		}

		images.convertTo(images, CV_8U);

		this->indexDataset = images;
	}

	return this->indexDataset;
}

const std::vector<std::pair<int, float>> &Companion::Model::Processing::ImageHashModel::getScores() const {
    return scores;
}
