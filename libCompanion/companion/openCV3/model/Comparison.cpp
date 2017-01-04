#include "Comparison.h"

Comparison::Comparison() {
}

Comparison::~Comparison() {
    image.release();
    descriptors.release();
}

const cv::Mat &Comparison::getImage() const {
    return image;
}

void Comparison::setImage(const cv::Mat &image) {
    Comparison::image = image;
}

const cv::Rect &Comparison::getLastPosition() const {
    return lastPosition;
}

void Comparison::setLastPosition(int x, int y, int width, int height) {
    lastPosition.x = x;
    lastPosition.y = y;
    lastPosition.width = width;
    lastPosition.height = height;
}

void Comparison::setLastPositionWidth(int width) {
    lastPosition.width = width;
}

void Comparison::setLastPositionHeight(int height) {
    lastPosition.height = height;
}

void Comparison::setLastPositionX(int x) {
    lastPosition.x = x;
}

void Comparison::setLastPositionY(int y) {
    lastPosition.y = y;
}

bool Comparison::isLastPositionSet() {
    return lastPosition.width > 0 && lastPosition.height > 0;
}

const cv::Mat &Comparison::getDescriptors() const {
    return descriptors;
}

void Comparison::setDescriptors(const cv::Mat &descriptors) {
    Comparison::descriptors = descriptors;
}

const std::vector<cv::KeyPoint> &Comparison::getKeypoints() const {
    return keypoints;
}

void Comparison::setKeypoints(const std::vector<cv::KeyPoint> &keypoints) {
    Comparison::keypoints = keypoints;
}