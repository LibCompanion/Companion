#include "ExportAPI.h"

/*********************/
/* Companion Wrapper */
/*********************/

Companion* createCompanion() {
    return new Companion();
}

void disposeCompanion(Companion* pObject) {
    if (pObject != NULL) {
        delete pObject;
        pObject = NULL;
    }
}

void callRun(Companion* pObject, StreamWorker &worker) {
    if (pObject != NULL) {
        pObject->run(worker);
    }
}

void callStop(Companion* pObject) {
    if (pObject != NULL) {
        pObject->stop();
    }
}

Stream* callGetSource(Companion* pObject) {
    if (pObject != NULL) {
        return pObject->getSource();
    } else {
        return NULL;
    }
}

void callSetSource(Companion* pObject, Stream* source) {
    if (pObject != NULL) {
        pObject->setSource(source);
    }
}

void callAddModel(Companion* pObject, ImageRecognitionModel* model) {
    if (pObject != NULL) {
        pObject->addModel(model);
    }
}

void callRemoveModel(Companion* pObject, ImageRecognitionModel* model) {
    if (pObject != NULL) {
        pObject->removeModel(model);
    }
}

void callClearModels(Companion* pObject) {
    if (pObject != NULL) {
        pObject->clearModels();
    }
}

ImageProcessing* callGetProcessing(Companion* pObject) {
    if (pObject != NULL) {
        return pObject->getProcessing();
    }
    else {
        return NULL;
    }
}

void callSetProcessing(Companion* pObject, ImageProcessing* processing) {
    if (pObject != NULL) {
        pObject->setProcessing(processing);
    }
}

int callGetSkipFrame(Companion* pObject) {
    if (pObject != NULL) {
        return pObject->getSkipFrame();
    } else {
        return 0;
    }
}

void callSetSkipFrame(Companion* pObject, int skipFrame) {
    if (pObject != NULL) {
        pObject->setSkipFrame(skipFrame);
    }
}

void callSetResultHandler(Companion* pObject, std::function<SUCCESS_CALLBACK> callback) {

}

void callSetErrorHandler(Companion* pObject, std::function<ERROR_CALLBACK> callback) {

}



Companion* createCompanionFake() {
    Companion* companion = new Companion();
    int type = cv::DescriptorMatcher::BRUTEFORCE_HAMMING;
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(type);

    cv::Ptr<cv::BRISK> feature = cv::BRISK::create(60);
    ImageRecognition *recognition = new CPUFeatureMatching(feature, feature, matcher, type, 40, true);

    companion->setProcessing(new ObjectDetection(companion, recognition, 0.50));
    companion->setSkipFrame(0);
    companion->setResultHandler(callback);
    companion->setErrorHandler(error);

    std::string path = "D:\\Downloads\\";
    std::vector<std::string> images;
    images.push_back(path + std::string("Sample_Middle.jpg"));
    images.push_back(path + std::string("Sample_Left.jpg"));
    images.push_back(path + std::string("Sample_Right.jpg"));
    std::string testVideo = path + std::string("Muelheim_HBF.mp4");

    Stream *stream = new Video(testVideo); // Load a video
    companion->setSource(stream);

    // Store all searched data models
    FeatureMatchingModel *object;
    for (auto &image : images) {
        object = new FeatureMatchingModel();
        object->setImage(cv::imread(image, cv::IMREAD_GRAYSCALE));

        // Only works on CPU -- ToDo Exception Handling if wrong type?
        //object->calculateKeyPointsAndDescriptors(feature, feature);

        if (!companion->addModel(object)) {
            std::cout << "Model not added";
        }
    }

    return companion;
    //return new Companion();
}

void callback(std::vector<Drawable*> objects, cv::Mat frame) {
    Drawable *drawable;

    for (int x = 0; x < objects.size(); x++) {
        drawable = objects.at(x);
        drawable->draw(frame);
    }

    cv::imshow("Object detection", frame);
    cv::waitKey(1);
    frame.release();
}

void error(CompanionError::errorCode code) {
    // Obtain detailed error message from code
    //std::cout << CompanionError::getError(code) << "\n";
}



/************************/
/* StreamWorker Wrapper */
/************************/
StreamWorker* createStreamWorker(std::queue<cv::Mat> &queue, int buffer) {
    return new StreamWorker(queue, buffer);
}

void disposeStreamWorker(StreamWorker* worker) {
    if (worker != NULL) {
        delete worker;
        worker = NULL;
    }
}

StreamWorker* createStreamWorkerFake(std::queue<cv::Mat>* queue, int buffer) {
    return new StreamWorker(*queue, buffer);
}



/********************/
/* Helper Functions */
/********************/
std::queue<cv::Mat>* createQueue() {
    std::queue<cv::Mat>* queue = new std::queue<cv::Mat>();
    return queue;
}

void disposeQueue(std::queue<cv::Mat>* queue) {
    if (queue != NULL) {
        delete queue;
        queue = NULL;
    }
}