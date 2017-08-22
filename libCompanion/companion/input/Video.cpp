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

#include "Video.h"

Companion::Input::Video::Video(int device) 
{

    cv::VideoCapture cap(device);

    if (!cap.isOpened()) 
	{
        throw Companion::Error::Code::invalid_video_src;
    }

    capture = cap;
    finished = false;
}


Companion::Input::Video::Video(std::string url) 
{

    cv::VideoCapture cap(url);

    if (!cap.isOpened()) 
	{
        throw Companion::Error::Code::invalid_video_src;
    }

    capture = cap;
    finished = false;
}

Companion::Input::Video::~Video() 
{
    // No need to close capture device because closing will be done from VideoCapture destructor.
}

cv::Mat Companion::Input::Video::obtainImage() 
{

    cv::Mat frame;

    if (!capture.isOpened() || finished) 
	{
        finished = true;
        return frame;
    }

    // Obtain image frame.
    capture >> frame;

    if (frame.empty()) 
	{
        // If frame empty video is finished.
        finished = true;
    }

    return frame;
}

bool Companion::Input::Video::isFinished() 
{
    return finished;
}

void Companion::Input::Video::finish() 
{
    finished = true;
}