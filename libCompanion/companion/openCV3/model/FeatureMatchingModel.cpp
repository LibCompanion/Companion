#include "FeatureMatchingModel.h"

FeatureMatchingModel::FeatureMatchingModel() {
}

FeatureMatchingModel::~FeatureMatchingModel() {
    descriptors.release();
}

const cv::Mat &FeatureMatchingModel::getDescriptors() const {
    return descriptors;
}

void FeatureMatchingModel::setDescriptors(const cv::Mat &descriptors) {
    FeatureMatchingModel::descriptors = descriptors;
}

const std::vector<cv::KeyPoint> &FeatureMatchingModel::getKeypoints() const {
    return keypoints;
}

void FeatureMatchingModel::setKeypoints(const std::vector<cv::KeyPoint> &keypoints) {
    FeatureMatchingModel::keypoints = keypoints;
}