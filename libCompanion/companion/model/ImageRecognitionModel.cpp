#include "ImageRecognitionModel.h"

Companion::Model::ImageRecognitionModel::ImageRecognitionModel() {
    ira = new Companion::Algorithm::IRA();
}

Companion::Model::ImageRecognitionModel::ImageRecognitionModel(cv::Mat image) {
    ImageRecognitionModel::image = image;
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
