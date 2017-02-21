#include "ObjectDetection.h"

ObjectDetection::ObjectDetection(Companion *companion, ImageRecognition *imageRecognition, float scale) {
    this->companion = companion;
    this->imageRecognition = imageRecognition;
    this->scale = scale;
}

ObjectDetection::~ObjectDetection() {}

std::vector<Drawable*> ObjectDetection::execute(cv::Mat frame) {

    FeatureMatchingModel *scene;
    Drawable *object;
    std::vector<Drawable*> objects;
    std::vector<ImageRecognitionModel *> models = companion->getModels();

    if (!frame.empty()) {
        scene = new FeatureMatchingModel();

        int oldX = frame.cols;
        int oldY = frame.rows;

        // https://antifreezedesign.wordpress.com/2011/05/13/permutations-of-1920x1080-for-perfect-scaling-at-1-77/
        Util::resizeImage(frame, oldX * scale, oldY * scale);
        scene->setImage(frame);

        // Check if image recognition implementation is an cuda implementation.
        if(imageRecognition->isCuda()) {
            // Cuda usage -> Don't use multithreading
            for(int x = 0; x < models.size(); x++) {
                object = imageRecognition->algo(scene, models.at(x));
                if(object != nullptr) {
                    // Create old image size
                    object->ratio(frame.cols, frame.rows, oldX, oldY);
                    // Store detected object to vector.
                    objects.push_back(object);
                }
            }
        } else {
            // Multithreading will be used for CPU usage
            #pragma omp parallel for
            for(int x = 0; x < models.size(); x++) {
                object = imageRecognition->algo(scene, models.at(x));
                if(object != nullptr) {
                    // Create old image size
                    object->ratio(frame.cols, frame.rows, oldX, oldY);
                    // Store detected object to vector.
                    objects.push_back(object);
                }
            }
        }

        delete scene;
    }

    return objects;
}