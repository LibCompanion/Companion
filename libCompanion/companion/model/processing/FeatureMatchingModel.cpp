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

#include "FeatureMatchingModel.h"

Companion::Model::Processing::FeatureMatchingModel::FeatureMatchingModel() {}

Companion::Model::Processing::FeatureMatchingModel::~FeatureMatchingModel() 
{
	keypoints.clear();
    descriptors.release();
}

const cv::Mat &Companion::Model::Processing::FeatureMatchingModel::getDescriptors() const 
{
    return descriptors;
}

void Companion::Model::Processing::FeatureMatchingModel::setDescriptors(const cv::Mat &descriptors) 
{
    FeatureMatchingModel::descriptors.empty();
    FeatureMatchingModel::descriptors = descriptors;
}

const std::vector<cv::KeyPoint> &Companion::Model::Processing::FeatureMatchingModel::getKeypoints() const 
{
    return keypoints;
}

void Companion::Model::Processing::FeatureMatchingModel::setKeypoints(const std::vector<cv::KeyPoint> &keypoints) 
{
	FeatureMatchingModel::keypoints.clear();
    FeatureMatchingModel::keypoints = keypoints;
}

void Companion::Model::Processing::FeatureMatchingModel::calculateKeyPointsAndDescriptors(cv::Ptr<cv::FeatureDetector> detector,
                                                            cv::Ptr<cv::DescriptorExtractor> extractor) 
{
	// Generates problems with detect and compute because image is not an mat object it is an gpu::mat
    keypoints.clear();
    descriptors.empty();
    detector->detect(image, keypoints);
    extractor->compute(image, keypoints, descriptors);
}

bool Companion::Model::Processing::FeatureMatchingModel::keypointsCalculated() 
{
    return !keypoints.empty();
}