#include "FeatureMatchingModel.h"

FeatureMatchingModel::FeatureMatchingModel() {
}

FeatureMatchingModel::~FeatureMatchingModel() {
	keypoints.clear();
    descriptors.release();
}

const cv::Mat &FeatureMatchingModel::getDescriptors() const {
    return descriptors;
}

void FeatureMatchingModel::setDescriptors(const cv::Mat &descriptors) {
    FeatureMatchingModel::descriptors.empty();
    FeatureMatchingModel::descriptors = descriptors;
}

const std::vector<cv::KeyPoint> &FeatureMatchingModel::getKeypoints() const {
    return keypoints;
}

void FeatureMatchingModel::setKeypoints(const std::vector<cv::KeyPoint> &keypoints) {
	FeatureMatchingModel::keypoints.clear();
    FeatureMatchingModel::keypoints = keypoints;
}

void FeatureMatchingModel::calculateKeyPointsAndDescriptors(cv::Ptr<cv::FeatureDetector> detector,
                                                            cv::Ptr<cv::DescriptorExtractor> extractor) {

    keypoints.clear();
    descriptors.empty();
    detector->detect(image, keypoints);
    extractor->compute(image, keypoints, descriptors);
}

bool FeatureMatchingModel::keypointsCalculated() {
    return !keypoints.empty();
}