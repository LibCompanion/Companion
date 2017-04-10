#include "ImageRecognitionModel.h"

ImageRecognitionModel::ImageRecognitionModel() {
    ira = new IRA();
}

ImageRecognitionModel::ImageRecognitionModel(cv::Mat image) {
    ImageRecognitionModel::image = image;
}

ImageRecognitionModel::~ImageRecognitionModel() {
    image.release();
    delete ira;
}

const cv::Mat &ImageRecognitionModel::getImage() const {
    return image;
}

void ImageRecognitionModel::setImage(const cv::Mat &image) {
    ImageRecognitionModel::image = image;
}

IRA *ImageRecognitionModel::getIra() const {
    return ira;
}
