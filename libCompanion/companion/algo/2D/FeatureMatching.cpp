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
        bool useIRA,
        double reprojThreshold,
        int ransacMaxIters,
        int findHomographyMethod) {

    this->detector = detector;
    this->extractor = extractor;
    this->matcherType = matcherType;
    this->matcher = matcher;
    this->cornerDistance = cornerDistance;
    this->countMatches = countMatches;
    this->useIRA = useIRA;
    this->cudaUsed = false;
    this->reprojThreshold = reprojThreshold;
    this->ransacMaxIters = ransacMaxIters;
    this->findHomographyMethod = findHomographyMethod;
}

#if Companion_USE_CUDA
Companion::Algorithm::FeatureMatching::FeatureMatching(
        cv::Ptr<cv::Feature2D> cudaFeatureMatching,
        int cornerDistance,
        int countMatches,
        double reprojThreshold,
        int ransacMaxIters,
        int findHomographyMethod) {

    this->cudaFeatureMatching = cudaFeatureMatching;
    this->cornerDistance = cornerDistance;
    this->countMatches = countMatches;
    this->useIRA = false;
    this->cudaUsed = true;
    this->reprojThreshold = reprojThreshold;
    this->ransacMaxIters = ransacMaxIters;
    this->findHomographyMethod = findHomographyMethod;
}
#endif

Companion::Algorithm::FeatureMatching::~FeatureMatching() {}

Companion::Model::Result *Companion::Algorithm::FeatureMatching::executeAlgorithm() {
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

        if (drawable != nullptr) {
            // TODO := SCORING CALCULATION
            // Object found
            result = new Companion::Model::Result(100, objectModel->getID(), drawable);
            sceneImage.release();
            objectImage.release();
        } else {
            // If result is not good enough and IRA was used.
            if (isIRAUsed) {

#if Companion_DEBUG
                showFeatureMatches(objectImage, keypointsObject, sceneImage, keypointsScene, goodMatches, "FM_NO_RESULT");
#endif

                sceneImage.release();
                objectImage.release();
                ira->clear(); // Clear last detected object position.
                return executeAlgorithm(); // Repeat algorithm to check original scene.
            } else {
#if Companion_DEBUG
                showFeatureMatches(objectImage, keypointsObject, sceneImage, keypointsScene, goodMatches, "FM_NO_RESULT");
#endif
            }
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
            sceneImage.release();
            objectImage.release();
        }
#endif

    } else {
        // If results are not good enough and empty for keypoints and descriptors.
        if(isIRAUsed) {
            sceneImage.release();
            objectImage.release();
            ira->clear(); // Clear last detected object position.
            return executeAlgorithm(); // Repeat algorithm to check original scene.
        }
    }

    return result;
}

bool Companion::Algorithm::FeatureMatching::isCuda() {
    return cudaUsed;
}

void Companion::Algorithm::FeatureMatching::ratio_test(const std::vector<std::vector<cv::DMatch>> &matches,
                                                std::vector<cv::DMatch> &good_matches,
                                                float ratio) {
    for (int i = 0; i < matches.size(); ++i) {
        if (matches[i].size() >= 2 && (matches[i][0].distance < ratio * matches[i][1].distance)) {
            good_matches.push_back(matches[i][0]);
        }
    }

}

void Companion::Algorithm::FeatureMatching::obtainKeypointsFromGoodMatches(
        std::vector<cv::DMatch> &good_matches,
        std::vector<cv::KeyPoint> &keypoints_object,
        std::vector<cv::KeyPoint> &keypoints_scene,
        std::vector<cv::Point2f> &feature_points_object,
        std::vector<cv::Point2f> &feature_points_scene) {

    int trainIdx;
    int queryIdx;

    // Get the keypoints from the good matches
    for (int i = 0; i < good_matches.size(); i++) {
        trainIdx = good_matches[i].trainIdx;
        queryIdx = good_matches[i].queryIdx;

        if ((trainIdx > 0 && keypoints_scene.size() > trainIdx) && (queryIdx > 0 && keypoints_object.size() > queryIdx)) {
            feature_points_scene.push_back(keypoints_scene[trainIdx].pt);
            feature_points_object.push_back(keypoints_object[queryIdx].pt);
        }
    }

}

Companion::Draw::Drawable* Companion::Algorithm::FeatureMatching::obtainMatchingResult(
        cv::Mat &sceneImage,
        cv::Mat &objectImage,
        std::vector<cv::DMatch> &good_matches,
        std::vector<cv::KeyPoint> &keypoints_object,
        std::vector<cv::KeyPoint> &keypoints_scene,
        Model::Processing::FeatureMatchingModel *sModel,
        Model::Processing::FeatureMatchingModel *cModel) {

    Companion::Draw::Drawable *drawable = nullptr;
    cv::Mat homography;
    std::vector<cv::Point2f> feature_points_object, feature_points_scene;

    feature_points_object.clear();
    feature_points_scene.clear();

    // Count of good matches if results are good enough.
    if (good_matches.size() > countMatches) {

        obtainKeypointsFromGoodMatches(good_matches,
                                       keypoints_object,
                                       keypoints_scene,
                                       feature_points_object,
                                       feature_points_scene);

        // Find Homography if only features points are filled.
        if(!feature_points_object.empty() && !feature_points_scene.empty()) {
            homography = cv::findHomography(feature_points_object, feature_points_scene, this->findHomographyMethod,
                                            this->reprojThreshold, cv::noArray(), this->ransacMaxIters);
            if (!homography.empty()) {
                drawable = calculateArea(homography, sceneImage, objectImage, sModel, cModel);
            }
        }

    }

    return drawable;
}

Companion::Draw::Drawable* Companion::Algorithm::FeatureMatching::calculateArea(
        cv::Mat &homography,
        cv::Mat &sceneImage,
        cv::Mat &objectImage,
        Model::Processing::FeatureMatchingModel *sModel,
        Model::Processing::FeatureMatchingModel *cModel) {

    Companion::Draw::Frame *frame = nullptr;
    cv::Mat originalImg = sModel->getImage();

    //-- Get the corners from the image_1 (the object to be "detected")
    std::vector<cv::Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0, 0);
    obj_corners[1] = cvPoint(objectImage.cols, 0);
    obj_corners[2] = cvPoint(objectImage.cols, objectImage.rows);
    obj_corners[3] = cvPoint(0, objectImage.rows);

    std::vector<cv::Point2f> scene_corners(4);
    cv::perspectiveTransform(obj_corners, scene_corners, homography);

    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    cv::Rect lastRect;
    IRA *ira = cModel->getIra();

    // IRA
    if(useIRA) {
        lastRect = ira->getLastObjectPosition();
    } else {
        lastRect = cv::Rect();
    }

    // Offset is recalculate position from last recognition if exists.
    cv::Point2f offset = cv::Point2f(lastRect.x, lastRect.y);

    // Focus area - Scene Corners
    //   0               1
    //   -----------------
    //   |               |
    //   |               |
    //   |               |
    //   |               |
    //   -----------------
    //   3               2

    float minX = sceneImage.cols;
    float maxX = 0;
    float minY = sceneImage.rows;
    float maxY = 0;

    for (cv::Point2f point : scene_corners) {
        if (point.x < minX) { minX = point.x; }
        if (point.x > maxX) { maxX = point.x; }
        if (point.y < minY) { minY = point.y; }
        if (point.y > maxY) { maxY = point.y; }
    }

    float width = maxX - minX;
    float height = maxY - minY;

    cv::Point2f scale = cv::Point2f(width / 2, height / 2);
    cv::Point2f start = cv::Point2f(minX, minY) + offset - scale;
    cv::Point2f end = cv::Point2f(maxX, maxY) + offset + scale;

    // Object area.

    cv::Point2f topLeft = scene_corners[0] + offset;
    cv::Point2f topRight = scene_corners[1] + offset;
    cv::Point2f bottomRight = scene_corners[2] + offset;
    cv::Point2f bottomLeft = scene_corners[3] + offset;

    // Check for minimum corner distance
    if (Companion::Util::hasDistantPosition(topLeft, topRight, this->cornerDistance) &&
        Companion::Util::hasDistantPosition(topLeft, bottomRight, this->cornerDistance) &&
        Companion::Util::hasDistantPosition(topLeft, bottomLeft, this->cornerDistance) &&
        Companion::Util::hasDistantPosition(topRight, bottomRight, this->cornerDistance) &&
        Companion::Util::hasDistantPosition(topRight, bottomLeft, this->cornerDistance) &&
        Companion::Util::hasDistantPosition(bottomRight, bottomLeft, this->cornerDistance)) {

        // Create a drawable frame to represent the calculated area
        frame = new Companion::Draw::Frame(topLeft, topRight, bottomLeft, bottomRight);
    }

    // If IRA is used...
    if(useIRA && frame != nullptr) {

        // IRA algo stores position from detected object.
        ira->setLastObjectPosition(start.x, start.y, end.x - start.x, end.y - start.y);
        const cv::Rect &lastObjectPosition = ira->getLastObjectPosition();

        // Check if start point is set correctly
        if (lastObjectPosition.x < 0) {
            ira->setX(0);
        }

        // Check if width is not oversized
        if (lastObjectPosition.width + lastObjectPosition.x > originalImg.cols) {
            ira->setWidth(originalImg.cols - lastObjectPosition.x);
        }

        if (lastObjectPosition.y < 0) {
            ira->setY(0);
        }

        if (lastObjectPosition.height + lastObjectPosition.y > originalImg.rows) {
            ira->setHeight(originalImg.rows - lastObjectPosition.y);
        }

        if (lastObjectPosition.area() <= 0) {
            // Something goes wrong in area clear IRA.
            ira->clear();
        }
    }

    return frame;
}

#if Companion_DEBUG
void Companion::Algorithm::FeatureMatching::showFeatureMatches(
                        cv::Mat& objectImg, std::vector<cv::KeyPoint>& objectKeypoints,
                        cv::Mat& sceneImg, std::vector<cv::KeyPoint>& sceneKeypoints,
                        std::vector<cv::DMatch>& goodMatches, std::string windowName) {
    // Build only in debug mode for development
    cv::Mat imgMatches;

    if(!objectImg.empty() && !sceneImg.empty()) {
        cv::drawMatches(objectImg, objectKeypoints, sceneImg, sceneKeypoints,
                        goodMatches, imgMatches, cv::Scalar::all(-1), cv::Scalar::all(-1),
                        std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

        //-- Show detected matches
        cv::imshow(windowName, imgMatches);
    }
}
#endif