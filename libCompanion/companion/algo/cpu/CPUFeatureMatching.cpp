#include "CPUFeatureMatching.h"

Companion::Algorithm::CPU::FeatureMatching::FeatureMatching() {

    this->detector = cv::ORB::create();
    this->extractor = cv::BRISK::create();
    this->type = cv::DescriptorMatcher::FLANNBASED;
    this->matcher = cv::DescriptorMatcher::create(type);
}

Companion::Algorithm::CPU::FeatureMatching::FeatureMatching(
        cv::Ptr<cv::FeatureDetector> detector,
        cv::Ptr<cv::DescriptorExtractor> extractor,
        cv::Ptr<cv::DescriptorMatcher> matcher,
        int type,
        int countMatches,
        bool useIRA) {

    this->detector = detector;
    this->extractor = extractor;
    this->type = type;
    this->matcher = matcher;
    this->countMatches = countMatches;
    this->useIRA = useIRA;

}

Companion::Algorithm::CPU::FeatureMatching::~FeatureMatching() {}

Companion::Draw::Drawable *Companion::Algorithm::CPU::FeatureMatching::algo(
        Model::ImageRecognitionModel *scene,
        Model::ImageRecognitionModel *object) {

    // Set of variables for feature matching.
    cv::Mat sceneImage, objectImage;
    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> goodMatches;
    std::vector<cv::KeyPoint> keypointsObject, keypointScene;
    cv::Mat descriptorsObject, descriptorsScene;
    Companion::Draw::Drawable *lines = nullptr;
    IRA* ira;
    bool isIRAUsed = false;

    Companion::Model::FeatureMatchingModel *sceneModel = dynamic_cast<Companion::Model::FeatureMatchingModel*>(scene);
    Companion::Model::FeatureMatchingModel *objectModel = dynamic_cast<Companion::Model::FeatureMatchingModel*>(object);

    // If wrong model types are used...
    if(!sceneModel || !objectModel) {
        throw Companion::Error::Code::wrong_model_type;
    }

    ira = objectModel->getIra();
    sceneImage = sceneModel->getImage();
    objectImage = objectModel->getImage();

    // TODO := Graysacle image
    cvtColor(sceneImage, sceneImage, CV_RGB2GRAY);

    // Check if images are loaded...
    if (!Util::isImageLoaded(sceneImage) || !Util::isImageLoaded(objectImage)) {
        throw Companion::Error::Code::image_not_found;
    }

    if (useIRA && ira->isObjectDetected()) {
        // IRA := Cut out scene from last detected object and set this as new scene to check.
        sceneImage = cv::Mat(sceneImage, ira->getLastObjectPosition());

        // Step 1 : Detect the keypoints from scene
        detector->detect(sceneImage, keypointScene);
        // Step 2 : Calculate descriptors (feature vectors)
        extractor->compute(sceneImage, keypointScene, descriptorsScene);

        isIRAUsed = true;
    }

    // Check if complete scene has calculated keypoints and descriptors and IRA was not used...
    if(!isIRAUsed && !sceneModel->keypointsCalculated()) {
        sceneModel->calculateKeyPointsAndDescriptors(detector, extractor);
    } else if(!isIRAUsed) {
        // Get Keypoints and descriptors from scene if IRA was not used.
        keypointScene = sceneModel->getKeypoints();
        descriptorsScene = sceneModel->getDescriptors();
    }

    // Check if object has calculated keypoints and descriptors...
    if(!objectModel->keypointsCalculated()) {
        objectModel->calculateKeyPointsAndDescriptors(detector, extractor);
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

        lines = obtainMatchingResult(sceneImage,
                                     objectImage,
                                     goodMatches,
                                     keypointsObject,
                                     keypointScene,
                                     sceneModel,
                                     objectModel);

        if(lines == nullptr) {
            // If result is not good enough and IRA was used.
            if(isIRAUsed) {
                ira->clear(); // Clear last detected object position.
                return algo(scene, object);
            }

        }

    } else {
        // Results from descriptors and keypoints are not good enough and IRA was used.
        if(isIRAUsed) {
            // Reset method and search for object in scene without IRA.
            ira->clear(); // Clear last detected object position.
            return algo(scene, object);
        }
    }

    return lines;
}

bool Companion::Algorithm::CPU::FeatureMatching::isCuda() {
    return false;
}