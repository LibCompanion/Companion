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

Companion::Model::ImageRecognitionModel::ImageRecognitionModel() {
    ira = new Companion::Algorithm::IRA();
}

Companion::Model::ImageRecognitionModel::ImageRecognitionModel(int id, cv::Mat image) : id(id), image(image) {
    
}

Companion::Model::ImageRecognitionModel::~ImageRecognitionModel() {
    image.release();
    delete ira;
}

const cv::Mat &Companion::Model::ImageRecognitionModel::getImage() const {
    return image;
}

void Companion::Model::ImageRecognitionModel::setImage(const cv::Mat &image) {
    ImageRecognitionModel::image = image;
}

Companion::Algorithm::IRA *Companion::Model::ImageRecognitionModel::getIra() const {
    return ira;
}

void Companion::Model::ImageRecognitionModel::setID(int id) {
    ImageRecognitionModel::id = id;
}

const int Companion::Model::ImageRecognitionModel::getID() const {
    return id;
}
