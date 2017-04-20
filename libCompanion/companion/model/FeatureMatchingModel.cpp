#include "FeatureMatchingModel.h"

Companion::Model::FeatureMatchingModel::FeatureMatchingModel() {
}

Companion::Model::FeatureMatchingModel::~FeatureMatchingModel() {
	keypoints.clear();
    descriptors.release();
}

const cv::Mat &Companion::Model::FeatureMatchingModel::getDescriptors() const {
    return descriptors;
}

void Companion::Model::FeatureMatchingModel::setDescriptors(const cv::Mat &descriptors) {
    FeatureMatchingModel::descriptors.empty();
    FeatureMatchingModel::descriptors = descriptors;
}

const std::vector<cv::KeyPoint> &Companion::Model::FeatureMatchingModel::getKeypoints() const {
    return keypoints;
}

void Companion::Model::FeatureMatchingModel::setKeypoints(const std::vector<cv::KeyPoint> &keypoints) {
	FeatureMatchingModel::keypoints.clear();
    FeatureMatchingModel::keypoints = keypoints;
}

void Companion::Model::FeatureMatchingModel::calculateKeyPointsAndDescriptors(cv::Ptr<cv::FeatureDetector> detector,
                                                            cv::Ptr<cv::DescriptorExtractor> extractor) {

    keypoints.clear();
    descriptors.empty();
    detector->detect(image, keypoints);
    extractor->compute(image, keypoints, descriptors);
}

bool Companion::Model::FeatureMatchingModel::keypointsCalculated() {
    return !keypoints.empty();
}