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

#include "Image.h"

Companion::Input::Image::Image(int maxImages)
{
	this->exitStream = false;
	this->exitAfterProcessing = false;
	this->maxImages = maxImages;
}

bool Companion::Input::Image::AddImage(std::string imgPath)
{
	return AddImage(cv::imread(imgPath));
}

bool Companion::Input::Image::AddImage(cv::Mat img)
{
	if (!img.empty())
	{
		// Limit queue size to keep memory low
		std::unique_lock<std::mutex> lk(this->mx);
		this->cv.wait(lk, [this] { return this->images.size() < this->maxImages; });
		// Stores only images which exist
		this->images.push(img);
		return true;
	}

	return false;
}

bool Companion::Input::Image::AddImage(int width, int height, int type, uchar* data)
{
	return AddImage(cv::Mat(cv::Size(width, height), type, data));
}

cv::Mat Companion::Input::Image::ObtainImage()
{
	cv::Mat image;
	std::unique_lock<std::mutex> lk(this->mx);
	if (!this->images.empty())
	{
		// Get first image from fifo
		image = this->images.front();
		this->images.pop();

		// Notify image input stream
		if (this->images.size() < this->maxImages)
		{
			this->cv.notify_one();
		}
	}

	return image;
}

bool Companion::Input::Image::IsFinished()
{
	std::unique_lock<std::mutex> lk(this->mx);
	return this->exitStream || (this->exitAfterProcessing && this->images.empty());
}

void Companion::Input::Image::Finish()
{
	this->exitStream = true;
}

void Companion::Input::Image::FinishAfterProcessing()
{
	this->exitAfterProcessing = true;
}
