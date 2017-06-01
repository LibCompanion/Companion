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

Companion::Model::Result* Companion::Algorithm::CPU::FeatureMatching::algo(
        Model::Processing::ImageRecognitionModel *sceneModel,
        Model::Processing::ImageRecognitionModel *objectModel) {

    // Set of variables for feature matching.
    cv::Mat sceneImage, objectImage;
    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> goodMatches;
    std::vector<cv::KeyPoint> keypointsObject, keypointScene;
    cv::Mat descriptorsObject, descriptorsScene;
    Companion::Model::Result *result = nullptr;
    Companion::Draw::Drawable *drawable = nullptr;
    IRA* ira;
    bool isIRAUsed = false;

    // Clear all lists from last run.
    matches.clear();
    goodMatches.clear();
    keypointScene.clear();
    keypointsObject.clear();

    Companion::Model::Processing::FeatureMatchingModel *sModel = dynamic_cast<Companion::Model::Processing::FeatureMatchingModel*>(sceneModel);
    Companion::Model::Processing::FeatureMatchingModel *oModel = dynamic_cast<Companion::Model::Processing::FeatureMatchingModel*>(objectModel);

    // If wrong model types are used...
    if(!sModel || !oModel) {
        throw Companion::Error::Code::wrong_model_type;
    }

    ira = oModel->getIra();
    sceneImage = sModel->getImage();
    objectImage = oModel->getImage();

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
    if(!isIRAUsed && !sModel->keypointsCalculated()) {
        sModel->calculateKeyPointsAndDescriptors(detector, extractor);
    }
    
    // Get Keypoints and descriptors from scene if IRA was not used.
    if(!isIRAUsed) {
        keypointScene = sModel->getKeypoints();
        descriptorsScene = sModel->getDescriptors();
    }

    // Check if object has calculated keypoints and descriptors...
    if(!oModel->keypointsCalculated()) {
        oModel->calculateKeyPointsAndDescriptors(detector, extractor);
    }

    // Get Keypoints and descriptors from object
    keypointsObject = oModel->getKeypoints();
    descriptorsObject = oModel->getDescriptors();

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

        drawable = obtainMatchingResult(sceneImage,
                                        objectImage,
                                        goodMatches,
                                        keypointsObject,
                                        keypointScene,
                                        sModel,
                                        oModel);

        if(drawable == nullptr) {
            // If result is not good enough and IRA was used.
            if(isIRAUsed) {
                ira->clear(); // Clear last detected object position.
                return algo(sceneModel, objectModel);
            }
        } else {
            // Object found...
            result = new Companion::Model::Result(100, objectModel->getID(), drawable);
        }

    } else {
        // Results from descriptors and keypoints are not good enough and IRA was used.
        if(isIRAUsed) {
            // Reset method and search for object in scene without IRA.
            ira->clear(); // Clear last detected object position.
            return algo(sceneModel, objectModel);
        }
    }

    sceneImage.release();
    objectImage.release();

    return result;
}

bool Companion::Algorithm::CPU::FeatureMatching::isCuda() {
    return false;
}