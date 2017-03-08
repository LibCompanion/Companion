#include "Companion.h"

Companion::Companion() {
    source = new Video();
    processing = nullptr;
    skipFrame = 0;
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

bool Companion::addModel(ImageRecognitionModel *model) {

    if(!model->getImage().empty()) {
        models.push_back(model);
        return true;
    }

    return false;
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

    if(skipFrame <= 0) {
        skipFrame = 0;
    }

    Companion::skipFrame = skipFrame;
}

void Companion::setResultHandler(std::function<void(std::vector<Drawable*>, cv::Mat)> callback) {
    Companion::callback = callback;
}

void Companion::setErrorHandler(std::function<void(CompanionError::errorCode)> callback) {
    Companion::errorCallback = callback;
}

void Companion::run(StreamWorker &worker) {
    this->consumer = std::thread(&StreamWorker::consume, &worker, this->getProcessing(), this->getErrorCallback(), this->getCallback());
    this->producer = std::thread(&StreamWorker::produce, &worker, this->getSource(), this->getSkipFrame(), this->getErrorCallback());
    consumer.join();
    producer.join();
}

const std::function<void(std::vector<Drawable *>, cv::Mat)> &Companion::getCallback() const {

    if(!callback) {
        throw CompanionError::errorCode::no_handler_set;
    }

    return callback;
}

const std::function<void(CompanionError::errorCode)> &Companion::getErrorCallback() const {

    if(!errorCallback) {
        throw CompanionError::errorCode::no_handler_set;
    }

    return errorCallback;
}
