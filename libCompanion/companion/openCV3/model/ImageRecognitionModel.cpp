#include "ImageRecognitionModel.h"

ImageRecognitionModel::ImageRecognitionModel() {}

ImageRecognitionModel::ImageRecognitionModel(cv::Mat image) {
    ImageRecognitionModel::image = image;
}

ImageRecognitionModel::~ImageRecognitionModel() {
    image.release();
}

const cv::Mat &ImageRecognitionModel::getImage() const {
    return image;
}

void ImageRecognitionModel::setImage(const cv::Mat &image) {
    ImageRecognitionModel::image = image;
}

const cv::Rect &ImageRecognitionModel::getLastPosition() const {
    return lastPosition;
}

void ImageRecognitionModel::setLastPosition(int x, int y, int width, int height) {
    lastPosition.x = x;
    lastPosition.y = y;
    lastPosition.width = width;
    lastPosition.height = height;
}

void ImageRecognitionModel::setLastPositionWidth(int width) {
    lastPosition.width = width;
}

void ImageRecognitionModel::setLastPositionHeight(int height) {
    lastPosition.height = height;
}

void ImageRecognitionModel::setLastPositionX(int x) {
    lastPosition.x = x;
}

void ImageRecognitionModel::setLastPositionY(int y) {
    lastPosition.y = y;
}

bool ImageRecognitionModel::isLastPositionSet() {
    return lastPosition.width != 0 && lastPosition.height != 0;
}