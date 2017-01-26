
#include "CompanionConfig.h"

CompanionConfig::CompanionConfig() {
    source = new Video();
    skipFrame = -1;
}

CompanionConfig::~CompanionConfig() {
    models.clear();
    delete source;
}

Video *CompanionConfig::getSource() const {
    return source;
}

void CompanionConfig::setSource(Video *source) {
    CompanionConfig::source = source;
}

void CompanionConfig::addModel(ImageRecognitionModel *model) {
    if(!model->getImage().empty()) {
        models.push_back(model);
    }
}

void CompanionConfig::removeModel(ImageRecognitionModel *model) {
    // ToDo Remove
}

void CompanionConfig::clearModels() {
    models.clear();
}

const std::vector<ImageRecognitionModel *> &CompanionConfig::getModels() const {
    return models;
}

ImageProcessing *CompanionConfig::getProcessing() const {
    return processing;
}

void CompanionConfig::setProcessing(ImageProcessing *processing) {
    CompanionConfig::processing = processing;
}

int CompanionConfig::getSkipFrame() const {
    return skipFrame;
}

void CompanionConfig::setSkipFrame(int skipFrame) {

    if(skipFrame < -1) {
        skipFrame = -1;
    }

    CompanionConfig::skipFrame = skipFrame;
}
