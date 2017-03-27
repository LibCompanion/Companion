#include "CPUFeatureMatching.h"

CPUFeatureMatching::CPUFeatureMatching() {

    this->detector = cv::ORB::create();
    this->extractor = cv::BRISK::create();
    this->type = cv::DescriptorMatcher::FLANNBASED;
    this->matcher = cv::DescriptorMatcher::create(type);
    this->countMatches = 40;
}

CPUFeatureMatching::CPUFeatureMatching(cv::Ptr<cv::FeatureDetector> detector, cv::Ptr<cv::DescriptorExtractor> extractor,
                                 cv::Ptr<cv::DescriptorMatcher> matcher, int type, int countMatches) {
    this->detector = detector;
    this->extractor = extractor;
    this->type = type;
    this->matcher = matcher;
    this->countMatches = countMatches;
}

CPUFeatureMatching::~CPUFeatureMatching() {}

Drawable *CPUFeatureMatching::algo(ImageRecognitionModel *scene, ImageRecognitionModel *object) {

    // Set of variables for feature matching.
    cv::Mat sceneImage, objectImage;
    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> goodMatches;
    std::vector<cv::KeyPoint> keypointsObject, keypointScene;
    cv::Mat descriptorsObject, descriptorsScene;
    Drawable *lines = nullptr;

    FeatureMatchingModel *sceneModel = dynamic_cast<FeatureMatchingModel*>(scene);
    FeatureMatchingModel *objectModel = dynamic_cast<FeatureMatchingModel*>(object);

    // If wrong model types are used...
    if(!sceneModel || !objectModel) {
        throw CompanionError::errorCode::wrong_model_type;
    }

    sceneImage = sceneModel->getImage();
    objectImage = objectModel->getImage();

    // TODO := Graysacle image
    cvtColor(sceneImage, sceneImage, CV_RGB2GRAY);

    // Check if images are loaded...
    if (!Util::isImageLoaded(sceneImage) || !Util::isImageLoaded(objectImage)) {
        throw CompanionError::errorCode::image_not_found;
    }

    bool isIRAUsed = false;

    // ToDO : IRA implementation class.
    if (objectModel->isLastPositionSet()) {
        // IRA := Object from last scene detected take this last position as scene.
        sceneImage = cv::Mat(sceneImage, objectModel->getLastPosition());

        // Step 1 : Detect the keypoints from scene
        detector->detect(sceneImage, keypointScene);
        // Step 2 : Calculate descriptors (feature vectors)
        extractor->compute(sceneImage, keypointScene, descriptorsScene);

        isIRAUsed = true;
    }

    // Check if scene has calculated keypoints and descriptors...
    if(!isIRAUsed && sceneModel->getKeypoints().empty()) {
        sceneModel->calculateKeyPointsAndDescriptors(detector, extractor);
    }

    // Check if object has calculated keypoints and descriptors...
    if(objectModel->getKeypoints().empty()) {
        objectModel->calculateKeyPointsAndDescriptors(detector, extractor);
    }

    // Get Keypoints and descriptors from scene if IRA was not used.
    if(!isIRAUsed) {
        keypointScene = sceneModel->getKeypoints();
        descriptorsScene = sceneModel->getDescriptors();
    }

    // Get Keypoints and descriptors from object
    keypointsObject = objectModel->getKeypoints();
    descriptorsObject = objectModel->getDescriptors();

    if (!descriptorsObject.empty() && !descriptorsScene.empty()
        && keypointsObject.size() > 0 && keypointScene.size() > 0) {

        // If matching type is flan based, scene and object must be in CV_32F format.
        if (type == cv::DescriptorMatcher::FLANNBASED) {
            descriptorsScene.convertTo(descriptorsScene, CV_32F);
            descriptorsObject.convertTo(descriptorsObject, CV_32F);
        }

        // Step 3: Matching descriptor vectors
        matcher->knnMatch(descriptorsObject, descriptorsScene, matches, 2);

        // Ratio test for good matches - http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
        // Neighbourhoods comparison
        ratio_test(matches, goodMatches, 0.80);

        // Symmetric matches
        // ToDo := How does it works?
        //vector<DMatch> symMatches;
        //symmetry_test(good_matches, good_matches_two, symMatches);
        //good_matches = symMatches;

        lines = obtainMatchingResult(sceneImage, objectImage, goodMatches, keypointsObject, keypointScene, sceneModel, objectModel);
        if(lines == nullptr) {
            // If result is not good enough and IRA was used.
            if(object->isLastPositionSet()) {
                object->setLastPosition(-1, -1, 0, 0); // Reset position because object is no more detected...
                return algo(scene, object);
            }
        }

    } else {
        // ToDo Method
        // If result is not good enough and IRA was used.
        if(object->isLastPositionSet()) {
            // Reset method
            object->setLastPosition(-1, -1, 0, 0); // Reset position because object is no more detected...
            return algo(scene, object);
        }
    }

    return lines;
}

bool CPUFeatureMatching::isCuda() {
    return false;
}