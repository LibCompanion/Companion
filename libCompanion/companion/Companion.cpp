#include "Companion.h"

Companion::Configuration::Configuration() {
    source = nullptr;
    processing = nullptr;
    skipFrame = 0;
}

Companion::Configuration::~Configuration() {
    models.clear();
    delete source;
    delete processing;
}

void Companion::Configuration::run(Companion::Thread::StreamWorker &worker) {
    this->worker = &worker;
    // Run new worker class.
    this->consumer = std::thread(&Thread::StreamWorker::consume, &worker, this->getProcessing(), this->getErrorCallback(), this->getCallback());
    this->producer = std::thread(&Thread::StreamWorker::produce, &worker, this->getSource(), this->getSkipFrame(), this->getErrorCallback());
    consumer.join();
    producer.join();
}

void Companion::Configuration::stop() {
    worker->stop();
}

Companion::Input::Stream *Companion::Configuration::getSource() const {

    if(!source) {
        throw Error::Code::video_src_not_set;
    }

    return source;
}

void Companion::Configuration::setSource(Companion::Input::Stream *source) {
    Configuration::source = source;
}

bool Companion::Configuration::addModel(Companion::Model::ImageRecognitionModel *model) {

    if(!model->getImage().empty()) {
        models.push_back(model);
        return true;
    }

    return false;
}

void Companion::Configuration::removeModel(Companion::Model::ImageRecognitionModel *model) {
    // ToDo Remove
}

void Companion::Configuration::clearModels() {
    models.clear();
}

const std::vector<Companion::Model::ImageRecognitionModel *> &Companion::Configuration::getModels() const {
    return models;
}

Companion::Processing::ImageProcessing *Companion::Configuration::getProcessing() const {

    if(processing == nullptr) {
        throw Error::Code::no_image_processing_algo_set;
    }

    return processing;
}

void Companion::Configuration::setProcessing(Companion::Processing::ImageProcessing *processing) {
    Configuration::processing = processing;
}

int Companion::Configuration::getSkipFrame() const {
    return skipFrame;
}

void Companion::Configuration::setSkipFrame(int skipFrame) {

    if(skipFrame <= 0) {
        skipFrame = 0;
    }

    Configuration::skipFrame = skipFrame;
}

void Companion::Configuration::setResultHandler(std::function<SUCCESS_CALLBACK> callback) {
    Configuration::callback = callback;
}

void Companion::Configuration::setErrorHandler(std::function<ERROR_CALLBACK> callback) {
    Configuration::errorCallback = callback;
}


const std::function<SUCCESS_CALLBACK> &Companion::Configuration::getCallback() const {

    if(!callback) {
        throw Error::Code::no_handler_set;
    }

    return callback;
}


const std::function<ERROR_CALLBACK> &Companion::Configuration::getErrorCallback() const {

    if(!errorCallback) {
        throw Error::Code::no_handler_set;
    }

    return errorCallback;
}
