/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2017 Andreas Sekulski
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Configuration.h"

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
    // Get all configuration data
    // Throws Error if invalid settings are set.
    Companion::Input::Stream* stream = this->getSource();
    Companion::Processing::ImageProcessing* imageProcessing = this->getProcessing();
    int skipFrame = this->getSkipFrame();
    std::function<ERROR_CALLBACK> errorCallback = this->getErrorCallback();
    std::function<SUCCESS_CALLBACK> successCallback = this->getCallback();

    // Run new worker class.
    this->worker = &worker;
    this->consumer = std::thread(&Thread::StreamWorker::consume, &worker, imageProcessing, errorCallback, successCallback);
    this->producer = std::thread(&Thread::StreamWorker::produce, &worker, stream, skipFrame, errorCallback);
    consumer.join(); // Wait if thread finished
    producer.join(); // Waif if thread finished
}

void Companion::Configuration::stop() {

    if (this->worker != nullptr && this->worker->isRunning()) {
        this->worker->stop();
    }

    this->worker = nullptr;
}

Companion::Input::Stream *Companion::Configuration::getSource() const {
    if(this->source == nullptr) {
        throw Error::Code::stream_src_not_set;
    }

    return source;
}

void Companion::Configuration::setSource(Companion::Input::Stream *source) {
    Configuration::source = source;
}

bool Companion::Configuration::addModel(Companion::Model::Processing::ImageRecognitionModel *model) {

    if(!model->getImage().empty()) {
        models.push_back(model);
        return true;
    }

    return false;
}

void Companion::Configuration::removeModel(Companion::Model::Processing::ImageRecognitionModel *model) {
    // ToDo Remove
}

void Companion::Configuration::clearModels() {
    models.clear();
}

const std::vector<Companion::Model::Processing::ImageRecognitionModel *> &Companion::Configuration::getModels() const {
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
