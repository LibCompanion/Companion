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

#include "ImageRecognitionModel.h"

Companion::Model::Processing::ImageRecognitionModel::ImageRecognitionModel()
{
    this->ira = new Companion::Algorithm::IRA();
}

Companion::Model::Processing::ImageRecognitionModel::ImageRecognitionModel(int id, cv::Mat image)
{
	this->id = id;
	this->image = image;
}

Companion::Model::Processing::ImageRecognitionModel::~ImageRecognitionModel()
{
    this->image.release();
	delete this->ira;
}

const cv::Mat &Companion::Model::Processing::ImageRecognitionModel::getImage() const
{
	return this->image;
}

void Companion::Model::Processing::ImageRecognitionModel::setImage(const cv::Mat &image)
{
    this->image = image;
}

Companion::Algorithm::IRA *Companion::Model::Processing::ImageRecognitionModel::getIra() const
{
	return this->ira;
}

void Companion::Model::Processing::ImageRecognitionModel::setID(int id)
{
	this->id = id;
}

const int Companion::Model::Processing::ImageRecognitionModel::getID() const
{
	return this->id;
}
