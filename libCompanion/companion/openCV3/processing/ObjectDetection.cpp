#include "ObjectDetection.h"

ObjectDetection::ObjectDetection(CompanionConfig *config, FeatureMatching *featureMatching) {
    this->config = config;
    this->featureMatching = featureMatching;
}

ObjectDetection::~ObjectDetection() {}

void ObjectDetection::execute(cv::Mat frame) {

    FeatureMatchingModel *scene;
    Drawable *drawable;
    std::vector<ImageRecognitionModel *> models = config->getModels();

    if (!frame.empty()) {
        scene = new FeatureMatchingModel();

        // https://antifreezedesign.wordpress.com/2011/05/13/permutations-of-1920x1080-for-perfect-scaling-at-1-77/
        Util::resizeImage(frame, 1024, 576);
        scene->setImage(frame);

        // Check for each model if it is in image...
        #pragma omp parallel for
        for(int x = 0; x < models.size(); x++) {
            drawable = featureMatching->algo(scene, models.at(x));
            if(drawable != nullptr) {
                drawable->draw(frame);
            }
        }

        // ToDo Callback handler

        cv::imshow("Object detection", frame);
        cv::waitKey(1);
        delete scene;
    }
}