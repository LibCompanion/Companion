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

#include "Image.h"

Companion::Input::Image::Image() {
}

Companion::Input::Image::~Image() {
}

void Companion::Input::Image::addImage(std::string imgPath) {
    addImage(cv::imread(imgPath));
}

void Companion::Input::Image::addImage(cv::Mat img) {
    if(!img.empty()) {
        mtx.lock();
        // Stores only img which exists.
        images.push(img);
        mtx.unlock();
    }
}

cv::Mat Companion::Input::Image::obtainImage() {

    cv::Mat image;

    mtx.lock();
    if(!images.empty()) {
        // Get first image from fifo
        image = images.front();
        images.pop();
    }
    mtx.unlock();

    return image;
}