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

#include "FeatureMatchingModel.h"

Companion::Model::Processing::FeatureMatchingModel::FeatureMatchingModel() {
	this->ira = new Companion::Algorithm::Recognition::Matching::UTIL::IRA();
}

Companion::Model::Processing::FeatureMatchingModel::~FeatureMatchingModel()
{
	this->keypoints.clear();
	this->descriptors.release();
	delete this->ira;
}

const cv::Mat &Companion::Model::Processing::FeatureMatchingModel::getDescriptors() const
{
    return this->descriptors;
}

void Companion::Model::Processing::FeatureMatchingModel::setDescriptors(const cv::Mat &descriptors)
{
    this->descriptors.empty();
    this->descriptors = descriptors;
}

const std::vector<cv::KeyPoint> &Companion::Model::Processing::FeatureMatchingModel::getKeypoints() const
{
    return this->keypoints;
}

void Companion::Model::Processing::FeatureMatchingModel::setKeypoints(const std::vector<cv::KeyPoint> &keypoints)
{
    this->keypoints.clear();
    this->keypoints = keypoints;
}

void Companion::Model::Processing::FeatureMatchingModel::calculateKeyPointsAndDescriptors(cv::Ptr<cv::FeatureDetector> detector,
    cv::Ptr<cv::DescriptorExtractor> extractor)
{
    // Generates problems with detect and compute because image is not an mat object it is an gpu::mat
    this->keypoints.clear();
    this->descriptors.empty();
    detector->detect(this->image, this->keypoints);
    extractor->compute(this->image, this->keypoints, this->descriptors);
}

bool Companion::Model::Processing::FeatureMatchingModel::keypointsCalculated()
{
    return !this->keypoints.empty();
}

const cv::Mat &Companion::Model::Processing::FeatureMatchingModel::getImage() const
{
	return this->image;
}

void Companion::Model::Processing::FeatureMatchingModel::setImage(const cv::Mat &image)
{
	this->image = image;
}

Companion::Algorithm::Recognition::Matching::UTIL::IRA *Companion::Model::Processing::FeatureMatchingModel::getIra() const
{
	return this->ira;
}

void Companion::Model::Processing::FeatureMatchingModel::setID(int id)
{
    this->id = id;
}

const int Companion::Model::Processing::FeatureMatchingModel::getID() const
{
	return this->id;
}
