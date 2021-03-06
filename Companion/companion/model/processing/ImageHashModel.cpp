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

#include "ImageHashModel.h"

Companion::Model::Processing::ImageHashModel::ImageHashModel()
{
	this->newModelAdded = true;
}

void Companion::Model::Processing::ImageHashModel::AddDescriptor(int id, cv::Mat& descriptor)
{
	this->imageDataset.push_back(descriptor);
	// Store this id for a scoring
	this->scores.push_back({ id, 0.0f });
	this->newModelAdded = true;
}

std::pair<cv::Mat_<float>, cv::Mat> Companion::Model::Processing::ImageHashModel::GenerateDataset()
{
	std::pair<cv::Mat_<float>, cv::Mat> dataset;
	dataset.first = GenerateHashImages();
	dataset.second = GenerateIndexDataset(dataset.first);
	this->newModelAdded = false;
	return dataset;
}

cv::Mat_<float> Companion::Model::Processing::ImageHashModel::GenerateHashImages()
{
	if (this->newModelAdded)
	{
		cv::Mat_<float> hash(this->imageDataset.cols, 100.0f);
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

cv::Mat Companion::Model::Processing::ImageHashModel::GenerateIndexDataset(cv::Mat_<float> hash)
{
	if (this->newModelAdded)
	{
		cv::Mat images = this->imageDataset * hash;

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

const std::vector<std::pair<int, float>>& Companion::Model::Processing::ImageHashModel::Scores() const
{
	return this->scores;
}
