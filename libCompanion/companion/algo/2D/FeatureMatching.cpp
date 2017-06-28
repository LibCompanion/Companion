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

#include "FeatureMatching.h"

Companion::Algorithm::FeatureMatching::FeatureMatching(
        cv::Ptr<cv::FeatureDetector> detector,
        cv::Ptr<cv::DescriptorExtractor> extractor,
        cv::Ptr<cv::DescriptorMatcher> matcher,
        int matcherType,
        int cornerDistance,
        int countMatches,
        bool useIRA) {

    this->detector = detector;
    this->extractor = extractor;
    this->matcherType = matcherType;
    this->matcher = matcher;
    this->cornerDistance = cornerDistance;
    this->countMatches = countMatches;
    this->useIRA = useIRA;
    this->cudaUsed = false;
}

#if Companion_USE_CUDA
Companion::Algorithm::FeatureMatching::FeatureMatching(cv::Ptr<cv::Feature2D> cudaFeatureMatching,
                                                       int cornerDistance,
                                                       int countMatches) {
    this->cudaFeatureMatching = cudaFeatureMatching;
    this->cornerDistance = cornerDistance;
    this->countMatches = countMatches;
    this->useIRA = false;
    this->cudaUsed = true;
}
#endif

Companion::Algorithm::FeatureMatching::~FeatureMatching() {}

Companion::Model::Result* Companion::Algorithm::FeatureMatching::algo(
        Model::Processing::ImageRecognitionModel *sceneModel,
        Model::Processing::ImageRecognitionModel *objectModel) {


    // Set of variables for feature matching.
    cv::Mat sceneImage, objectImage;
    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> goodMatches;
    std::vector<cv::KeyPoint> keypointsScene, keypointsObject;
    cv::Mat descriptorsScene, descriptorsObject;
    Companion::Model::Result *result = nullptr;
    Companion::Draw::Drawable *drawable = nullptr;
    bool isIRAUsed = false;
    IRA* ira;
    Companion::Model::Processing::FeatureMatchingModel* sModel;
    Companion::Model::Processing::FeatureMatchingModel* oModel;

    // Clear all lists from last run.
    matches.clear();
    goodMatches.clear();
    keypointsScene.clear();
    keypointsObject.clear();

    // Type cast feature matching model to specific one and validate it.
    sModel = dynamic_cast<Companion::Model::Processing::FeatureMatchingModel*>(sceneModel);
    oModel = dynamic_cast<Companion::Model::Processing::FeatureMatchingModel*>(objectModel);

    if(!sModel || !oModel) {
        // If wrong model types are used
        throw Companion::Error::Code::wrong_model_type;
    }

    ira = oModel->getIra();  // Get IRA from object model
    sceneImage = sModel->getImage(); // Get image scene
    objectImage = oModel->getImage(); // Get object scene
    cvtColor(sceneImage, sceneImage, CV_BGR2GRAY); // Convert image to grayscale

    // Check if images are loaded...
    if (!Util::isImageLoaded(sceneImage) || !Util::isImageLoaded(objectImage)) {
        throw Companion::Error::Code::image_not_found;
    }

    // --------------------------------------------------
    // Scene and model preparation start
    // --------------------------------------------------

    // ------ IRA scene handling. Currently works only for CPU usage ------
    if (useIRA && ira->isObjectDetected()) { // IRA cut out handling if ira should be used and object was detected.
        // Cut out scene from last detected object and set this as new scene to check.
        sceneImage = cv::Mat(sceneImage, ira->getLastObjectPosition());
        // Detect keypoints from cut scene
        detector->detect(sceneImage, keypointsScene);
        // Calculate descriptors from cut scene (feature vectors)
        extractor->compute(sceneImage, keypointsScene, descriptorsScene);
        isIRAUsed = true;
    } else if(useIRA && sModel->keypointsCalculated()) { // If IRA is used and keypoints from scene already calculated.
        keypointsScene = sModel->getKeypoints();
        descriptorsScene = sModel->getDescriptors();
    } else if(!cudaUsed) { // If IRA is not used or keypoints from scene are not calculated yet.
        sModel->calculateKeyPointsAndDescriptors(detector, extractor); // Calculate keypoints
        keypointsScene = sModel->getKeypoints();
        descriptorsScene = sModel->getDescriptors();
    }

    // Check if object has calculated keypoints and descriptors and CUDA is not used.
    if(!oModel->keypointsCalculated() && !cudaUsed) {
        oModel->calculateKeyPointsAndDescriptors(detector, extractor); // Calculate keypoints from model.
    }

    // Get Keypoints and descriptors from model.
    keypointsObject = oModel->getKeypoints();
    descriptorsObject = oModel->getDescriptors();

    // --------------------------------------------------
    // Scene and model preparation end
    // --------------------------------------------------

    // --------------------------------------------------
    // Feature matching algorithm
    // --------------------------------------------------
    // If object and scene descriptor and keypoints exists..
    if (!cudaUsed && !descriptorsObject.empty() && !descriptorsScene.empty() && keypointsObject.size() > 0 && keypointsScene.size() > 0) {

        // ------ CPU USAGE ------

        // If matching type is flan based, scene and object must be in CV_32F format.
        if (matcherType == cv::DescriptorMatcher::FLANNBASED) {
            descriptorsScene.convertTo(descriptorsScene, CV_32F);
            descriptorsObject.convertTo(descriptorsObject, CV_32F);
        }

        // Matching descriptor vectors
        matcher->knnMatch(descriptorsObject, descriptorsScene, matches, 2);

        // --------------------------------------------------
        // Feature matching result filtering
        // --------------------------------------------------

        // Ratio test for good matches - http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
        // Neighbourhoods comparison
        ratio_test(matches, goodMatches, 0.80);

        drawable = obtainMatchingResult(sceneImage,
                                        objectImage,
                                        goodMatches,
                                        keypointsObject,
                                        keypointsScene,
                                        sModel,
                                        oModel);

        if(drawable == nullptr) {
            // If result is not good enough and IRA was used.
            if(isIRAUsed) {
                sceneImage.release();
                objectImage.release();
                ira->clear(); // Clear last detected object position.
                return algo(sceneModel, objectModel); // Repeat algorithm to check original scene.
            }
        } else {
            // TODO := SCORING CALCULATION
            // Object found
            result = new Companion::Model::Result(100, objectModel->getID(), drawable);
            sceneImage.release();
            objectImage.release();
        }


    } else if(cudaUsed) {

        #if Companion_USE_CUDA

        if(cv::cuda::getCudaEnabledDeviceCount() == 0) {
            throw Companion::Error::Code::no_cuda_device;
        }

        // ------ Cuda USAGE ------
        cv::cuda::GpuMat gpu_scene(sceneImage); // Load scene as an gpu mat
        cv::cuda::GpuMat gpu_object(objectImage); // Load object as an gpu mat
        cv::cuda::GpuMat gpu_descriptors_scene, gpu_descriptors_object;

        cudaFeatureMatching->detectAndCompute(gpu_scene, cv::noArray(), keypointsScene, gpu_descriptors_scene);
        cudaFeatureMatching->detectAndCompute(gpu_object, cv::noArray(), keypointsObject, gpu_descriptors_object);

        cv::Ptr<cv::cuda::DescriptorMatcher> gpu_matcher = cv::cuda::DescriptorMatcher::createBFMatcher(cudaFeatureMatching->defaultNorm());

        gpu_matcher->knnMatch(gpu_descriptors_object, gpu_descriptors_scene, matches, 2);

        gpu_scene.release();
        gpu_object.release();

        // Ratio test for good matches - http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
        // Neighbourhoods comparison
        ratio_test(matches, goodMatches, 0.80);

        drawable = obtainMatchingResult(sceneImage,
                                        objectImage,
                                        goodMatches,
                                        keypointsObject,
                                        keypointsScene,
                                        sModel,
                                        oModel);

        if(drawable != nullptr) {
            // TODO := SCORING CALCULATION
            // Object found
            result = new Companion::Model::Result(100, oModel->getID(), drawable);
        }

        #endif

    } else {
        // Results from descriptors and keypoints are not good enough and IRA was used.
        if(isIRAUsed) {
            sceneImage.release();
            objectImage.release();
            ira->clear(); // Clear last detected object position.
            return algo(sceneModel, objectModel); // Repeat algorithm to check original scene.
        }
    }

    return result;
}

bool Companion::Algorithm::FeatureMatching::isCuda() {
    return cudaUsed;
}