#include "CPUFeatureMatching.h"

FeatureMatching::FeatureMatching() {

    this->detector = cv::ORB::create();
    this->extractor = cv::BRISK::create();
    this->type = cv::DescriptorMatcher::FLANNBASED;
    this->matcher = cv::DescriptorMatcher::create(type);
    this->countMatches = 40;
}

FeatureMatching::FeatureMatching(cv::Ptr<cv::FeatureDetector> detector, cv::Ptr<cv::DescriptorExtractor> extractor,
                                 cv::Ptr<cv::DescriptorMatcher> matcher, int type, int countMatches) {
    this->detector = detector;
    this->extractor = extractor;
    this->type = type;
    this->matcher = matcher;
    this->countMatches = countMatches;
}

FeatureMatching::~FeatureMatching() {}

Drawable *FeatureMatching::algo(ImageRecognitionModel *searchModel, ImageRecognitionModel *compareModel) {

    // Set of variables for feature matching.
    cv::Mat sceneImage, objectImage;
    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> good_matches;
    std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
    cv::Mat descriptors_object, descriptors_scene;
    Drawable *lines = nullptr;

    FeatureMatchingModel *sModel = dynamic_cast<FeatureMatchingModel *>(searchModel);
    FeatureMatchingModel *cModel = dynamic_cast<FeatureMatchingModel *>(compareModel);

    // If wrong model types are used...
    if(!sModel || !cModel) {
        throw CompanionError::errorCode::wrong_model_type;
    }

    sceneImage = sModel->getImage();
    objectImage = cModel->getImage();

    // Graysacle image TODO
    cvtColor(sceneImage, sceneImage, CV_RGB2GRAY);

    // Check if images are loaded...
    if (!Util::isImageLoaded(sceneImage) || !Util::isImageLoaded(objectImage)) {
        throw CompanionError::errorCode::image_not_found;
    }

    // ToDO : IRA implementation class.
    // Check if from last scene object was detected...
    if (compareModel->isLastPositionSet()) {
        // Object from last scene detected take this last position as scene.
        sceneImage = cv::Mat(sceneImage, compareModel->getLastPosition());
        // ToDo -> Methods
        // Step 1 : Detect the keypoints from scene
        detector->detect(sceneImage, keypoints_scene);
        // 2 : Calculate descriptors (feature vectors)
        extractor->compute(sceneImage, keypoints_scene, descriptors_scene);
    } else {
        // --- ToDo Method implementation ---
        // If model has already keypoints calculated from object...
        if(sModel->getKeypoints().empty()) {
            // Step 1 : Detect the keypoints
            detector->detect(sceneImage, keypoints_scene);
            sModel->setKeypoints(keypoints_scene);
            // Step 2 : Calculate descriptors (feature vectors)
            extractor->compute(sceneImage, keypoints_scene, descriptors_scene);
            sModel->setDescriptors(descriptors_scene);
        } else {
            // Keypoints and descriptors already calculated from model
            keypoints_scene = sModel->getKeypoints();
            descriptors_scene = sModel->getDescriptors();
        }
    }

    // --- ToDo Method implementation ---
    // If model has already keypoints calculated from object...
    if(cModel->getKeypoints().empty()) {
        // Step 1 : Detect the keypoints
        detector->detect(objectImage, keypoints_object);
        cModel->setKeypoints(keypoints_object);
        // Step 2 : Calculate descriptors (feature vectors)
        extractor->compute(objectImage, keypoints_object, descriptors_object);
        cModel->setDescriptors(descriptors_object);
    } else {
        // Keypoints and descriptors already calculated from model
        keypoints_object = cModel->getKeypoints();
        descriptors_object = cModel->getDescriptors();
    }

    if (!descriptors_object.empty() && !descriptors_scene.empty()
        && keypoints_object.size() > 0 && keypoints_scene.size() > 0) {

        // If matching type is flan based, scene and object must be in CV_32F format.
        if (type == cv::DescriptorMatcher::FLANNBASED) {
            descriptors_scene.convertTo(descriptors_scene, CV_32F);
            descriptors_object.convertTo(descriptors_object, CV_32F);
        }

        // Step 3: Matching descriptor vectors
        matcher->knnMatch(descriptors_object, descriptors_scene, matches, 2);

        // Ratio test for good matches - http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
        // Neighbourhoods comparison
        ratio_test(matches, good_matches, 0.80);

        // Symmetric matches
        // ToDo := How does it works?
        //vector<DMatch> symMatches;
        //symmetry_test(good_matches, good_matches_two, symMatches);
        //good_matches = symMatches;

        lines = obtainMatchingResult(sceneImage, objectImage, good_matches, keypoints_object, keypoints_scene, sModel, cModel);
        if(lines == nullptr) {
            // If result is not good enough and IRA was used.
            if(compareModel->isLastPositionSet()) {
                compareModel->setLastPosition(-1, -1, 0, 0); // Reset position because object is no more detected...
                return algo(searchModel, compareModel);
            }
        }

    } else {
        // If result is not good enough and IRA was used.
        if(compareModel->isLastPositionSet()) {
            compareModel->setLastPosition(-1, -1, 0, 0); // Reset position because object is no more detected...
            return algo(searchModel, compareModel);
        }
    }

    return lines;
}

bool FeatureMatching::isCuda() {
    return false;
}