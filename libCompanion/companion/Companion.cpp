#include "Companion.h"

Companion::Companion() {
    source = new Video();
    processing = nullptr;
    skipFrame = -1;
}

Companion::~Companion() {
    models.clear();
    delete source;
    delete processing;
}

Video *Companion::getSource() const {

    if(!source->isOpen()) {
        throw CompanionError::errorCode::video_src_not_set;
    }

    return source;
}

void Companion::setSource(Video *source) {
    Companion::source = source;
}

void Companion::addModel(ImageRecognitionModel *model) {
    if(!model->getImage().empty()) {
        models.push_back(model);
    }
}

void Companion::removeModel(ImageRecognitionModel *model) {
    // ToDo Remove
}

void Companion::clearModels() {
    models.clear();
}

const std::vector<ImageRecognitionModel *> &Companion::getModels() const {
    return models;
}

ImageProcessing *Companion::getProcessing() const {

    if(processing == nullptr) {
        throw CompanionError::errorCode::no_image_processing_algo_set;
    }

    return processing;
}

void Companion::setProcessing(ImageProcessing *processing) {
    Companion::processing = processing;
}

int Companion::getSkipFrame() const {
    return skipFrame;
}

void Companion::setSkipFrame(int skipFrame) {

    if(skipFrame < -1) {
        skipFrame = -1;
    }

    Companion::skipFrame = skipFrame;
}

void Companion::setResultHandler(std::function<void(std::vector<Drawable*>, cv::Mat)> callback) {
    Companion::callback = callback;
}

void Companion::executeResultHandler(std::vector<Drawable*> objects, cv::Mat frame) {
    if(callback) {
        callback(objects, frame);
    }
}

void Companion::setErrorHandler(std::function<void(CompanionError::errorCode)> callback) {
    Companion::errorCallback = callback;
}

void Companion::executeError(CompanionError::errorCode code) {
    if(errorCallback) {
        errorCallback(code);
    }
}