#include "ExportAPI.h"

/*********************/
/* Companion Wrapper */
/*********************/

Companion::Configuration* Companion::ExportAPI::createConfiguration() {
    return new Companion::Configuration();
}

void Companion::ExportAPI::disposeConfiguration(Companion::Configuration* config) {
    if (config != NULL) {
        delete config;
        config = NULL;
    }
}

void Companion::ExportAPI::callRun(Companion::Configuration* config, Companion::Thread::StreamWorker &worker) {
    if (config != NULL) {
        config->run(worker);
    }
}

void Companion::ExportAPI::callStop(Companion::Configuration* config) {
    if (config != NULL) {
        config->stop();
    }
}

Companion::Input::Stream* Companion::ExportAPI::callGetSource(Companion::Configuration* config) {
    if (config != NULL) {
        return config->getSource();
    } else {
        return NULL;
    }
}

void Companion::ExportAPI::callSetSource(Companion::Configuration* config, Companion::Input::Stream* source) {
    if (config != NULL) {
        config->setSource(source);
    }
}

void Companion::ExportAPI::callAddModel(Companion::Configuration* config, Companion::Model::ImageRecognitionModel* model) {
    if (config != NULL) {
        config->addModel(model);
    }
}

void Companion::ExportAPI::callRemoveModel(Companion::Configuration* config, Companion::Model::ImageRecognitionModel* model) {
    if (config != NULL) {
        config->removeModel(model);
    }
}

void Companion::ExportAPI::callClearModels(Companion::Configuration* config) {
    if (config != NULL) {
        config->clearModels();
    }
}

Companion::Processing::ImageProcessing* Companion::ExportAPI::callGetProcessing(Companion::Configuration* config) {
    if (config != NULL) {
        return config->getProcessing();
    }
    else {
        return NULL;
    }
}

void Companion::ExportAPI::callSetProcessing(Companion::Configuration* config, Companion::Processing::ImageProcessing* processing) {
    if (config != NULL) {
        config->setProcessing(processing);
    }
}

int Companion::ExportAPI::callGetSkipFrame(Companion::Configuration* config) {
    if (config != NULL) {
        return config->getSkipFrame();
    } else {
        return 0;
    }
}

void Companion::ExportAPI::callSetSkipFrame(Companion::Configuration* config, int skipFrame) {
    if (config != NULL) {
        config->setSkipFrame(skipFrame);
    }
}

void Companion::ExportAPI::callSetResultHandler(Companion::Configuration* config, std::function<SUCCESS_CALLBACK> callback) {

}

void Companion::ExportAPI::callSetErrorHandler(Companion::Configuration* config, std::function<ERROR_CALLBACK> callback) {

}



Companion::Configuration* Companion::ExportAPI::createCompanionFake() {
    Companion::Configuration* config = new Companion::Configuration();
    int type = cv::DescriptorMatcher::BRUTEFORCE_HAMMING;
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(type);

    cv::Ptr<cv::BRISK> feature = cv::BRISK::create(60);
    Companion::Algorithm::ImageRecognition *recognition = new Companion::Algorithm::CPU::FeatureMatching(feature, feature, matcher, type, 40, true);

    config->setProcessing(new Companion::Processing::ObjectDetection(config, recognition, 0.50));
    config->setSkipFrame(0);
    config->setResultHandler(Companion::ExportAPI::callback);
    config->setErrorHandler(Companion::ExportAPI::error);

    std::string path = "D:\\Downloads\\";
    std::vector<std::string> images;
    images.push_back(path + std::string("Sample_Middle.jpg"));
    images.push_back(path + std::string("Sample_Left.jpg"));
    images.push_back(path + std::string("Sample_Right.jpg"));
    std::string testVideo = path + std::string("Muelheim_HBF.mp4");

    Companion::Input::Stream *stream = new Companion::Input::Video(testVideo); // Load a video
    config->setSource(stream);

    // Store all searched data models
    Companion::Model::FeatureMatchingModel *object;
    for (auto &image : images) {
        object = new Companion::Model::FeatureMatchingModel();
        object->setImage(cv::imread(image, cv::IMREAD_GRAYSCALE));
    }

    return config;
}

void Companion::ExportAPI::callback(std::vector<Companion::Draw::Drawable*> objects, cv::Mat frame) {
    Companion::Draw::Drawable *drawable;

    for (int x = 0; x < objects.size(); x++) {
        drawable = objects.at(x);
        drawable->draw(frame);
    }

    cv::imshow("Object detection", frame);
    cv::waitKey(1);
    frame.release();
}

void Companion::ExportAPI::error(Companion::Error::Code code) {
    // Obtain detailed error message from code
    //std::cout << Companion::Error::getError(code) << "\n";
}



/************************/
/* StreamWorker Wrapper */
/************************/
Companion::Thread::StreamWorker* Companion::ExportAPI::createStreamWorker(std::queue<cv::Mat> &queue, int buffer) {
    return new Companion::Thread::StreamWorker(queue, buffer);
}

void Companion::ExportAPI::disposeStreamWorker(Companion::Thread::StreamWorker* worker) {
    if (worker != NULL) {
        delete worker;
        worker = NULL;
    }
}

Companion::Thread::StreamWorker* Companion::ExportAPI::createStreamWorkerFake(std::queue<cv::Mat>* queue, int buffer) {
    return new Companion::Thread::StreamWorker(*queue, buffer);
}



/********************/
/* Helper Functions */
/********************/
std::queue<cv::Mat>* Companion::ExportAPI::createQueue() {
    std::queue<cv::Mat>* queue = new std::queue<cv::Mat>();
    return queue;
}

void Companion::ExportAPI::disposeQueue(std::queue<cv::Mat>* queue) {
    if (queue != NULL) {
        delete queue;
        queue = NULL;
    }
}