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
    int minSidelLength,
    int countMatches,
    bool useIRA,
    double reprojThreshold,
    int ransacMaxIters,
    int findHomographyMethod)
{
    this->detector = detector;
    this->extractor = extractor;
    this->matcherType = matcherType;
    this->matcher = matcher;
    this->minSidelLength = minSidelLength;
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
    int minSidelLength,
    int countMatches,
    double reprojThreshold,
    int ransacMaxIters,
    int findHomographyMethod)
{

    this->cudaFeatureMatching = cudaFeatureMatching;
    this->minSidelLength = minSidelLength;
    this->countMatches = countMatches;
    this->useIRA = false;
    this->cudaUsed = true;
    this->reprojThreshold = reprojThreshold;
    this->ransacMaxIters = ransacMaxIters;
    this->findHomographyMethod = findHomographyMethod;
}
#endif

Companion::Algorithm::FeatureMatching::~FeatureMatching() {}

Companion::Model::Result *Companion::Algorithm::FeatureMatching::executeAlgorithm(
    Model::Processing::FeatureMatchingModel *sceneModel,
    Model::Processing::FeatureMatchingModel *objectModel,
    Companion::Draw::Frame *roi)
{

    // Set of variables for feature matching.
    cv::Mat sceneImage, objectImage;
    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> goodMatches;
    std::vector<cv::KeyPoint> keypointsScene, keypointsObject;
    cv::Mat descriptorsScene, descriptorsObject;
    Companion::Model::Result *result = nullptr;
    Companion::Draw::Drawable *drawable = nullptr;
    bool isIRAUsed = false;
    bool isROIUsed = false;
    Companion::Algorithm::IRA* ira;

    // Clear all lists from last run.
    matches.clear();
    goodMatches.clear();
    keypointsScene.clear();
    keypointsObject.clear();

    sceneImage = sceneModel->getImage(); // Get image scene
    objectImage = objectModel->getImage(); // Get object scene
    ira = objectModel->getIra();  // Get IRA from object model

    // Check if images are loaded...
    if (!Util::isImageLoaded(sceneImage) || !Util::isImageLoaded(objectImage))
    {
        throw Companion::Error::Code::image_not_found;
    }

    cvtColor(sceneImage, sceneImage, CV_BGR2GRAY); // Convert image to grayscale

    // --------------------------------------------------
    // Scene and model preparation start
    // --------------------------------------------------

    // ------ IRA scene handling. Currently works only for CPU usage ------
    if (this->useIRA && ira->isObjectDetected()) // IRA USED & OBJECT DETECTED
    {
        // Cut out scene from last detected object and set this as new scene to check.
        sceneImage = cv::Mat(sceneImage, ira->getLastObjectPosition());

        // Detect keypoints from cut scene
        this->detector->detect(sceneImage, keypointsScene);
        // Calculate descriptors from cut scene (feature vectors)
        this->extractor->compute(sceneImage, keypointsScene, descriptorsScene);

        isIRAUsed = true;
    }
    else if (this->useIRA && roi != nullptr) // IRA USED & OBJECT NOT DETECTED & ROI EXISTS
    {
        // Get ROI position
        cv::Rect roiObject(roi->getTopLeft(), roi->getBottomRight());

        // Cut out scene from last detected object and set this as new scene to check.
        sceneImage = cv::Mat(sceneImage, roiObject);

        // Detect keypoints from cut scene
        this->detector->detect(sceneImage, keypointsScene);
        // Calculate descriptors from cut scene (feature vectors)
        this->extractor->compute(sceneImage, keypointsScene, descriptorsScene);

        isROIUsed = true;
    }
    else if (this->useIRA && sceneModel->keypointsCalculated()) // IRA USED & OBJECT NOT DETECTED & SCENE KEYPOINTS CALCULATED
    {
        keypointsScene = sceneModel->getKeypoints();
        descriptorsScene = sceneModel->getDescriptors();
    }
    else if (!this->cudaUsed) // IRA NOT USED & OBJECT NOT DETECTED & SCENE KEYPOINTS NOT CALCULATED & NOT CUDA USAGE
    {
        // Detect keypoints
        this->detector->detect(sceneImage, keypointsScene);
        // Calculate descriptors
        this->extractor->compute(sceneImage, keypointsScene, descriptorsScene);
    }

    // Check if object has calculated keypoints and descriptors and CUDA is not used.
    if (!objectModel->keypointsCalculated() && !this->cudaUsed)
    {
        objectModel->calculateKeyPointsAndDescriptors(this->detector, this->extractor); // Calculate keypoints from model.
    }

    // Get Keypoints and descriptors from model.
    keypointsObject = objectModel->getKeypoints();
    descriptorsObject = objectModel->getDescriptors();

    // --------------------------------------------------
    // Scene and model preparation end
    // --------------------------------------------------

    // --------------------------------------------------
    // Feature matching algorithm
    // --------------------------------------------------
    // If object and scene descriptor and keypoints exists..
    if (!this->cudaUsed && !descriptorsObject.empty() && !descriptorsScene.empty() && !keypointsObject.empty() && !keypointsScene.empty())
    {
        // If matching type is flan based, scene and object must be in CV_32F format.
        if (this->matcherType == cv::DescriptorMatcher::FLANNBASED)
        {
            descriptorsScene.convertTo(descriptorsScene, CV_32F);
            descriptorsObject.convertTo(descriptorsObject, CV_32F);
        }

        // ------ CPU USAGE ------
        // Matching descriptor vectors
        this->matcher->knnMatch(descriptorsObject, descriptorsScene, matches, DEFAULT_NEIGHBOR);

        // Ratio test for good matches - http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
        // Neighbourhoods comparison
        ratioTest(matches, goodMatches, DEFAULT_RATIO_VALUE);

        drawable = obtainMatchingResult(sceneImage,
            objectImage,
            goodMatches,
            keypointsObject,
            keypointsScene,
            sceneModel,
            objectModel,
            isIRAUsed,
            isROIUsed,
            roi);
    }
    #if Companion_USE_CUDA
    else if (this->cudaUsed)
    {
        // ------ Cuda USAGE ------
        if (cv::cuda::getCudaEnabledDeviceCount() == 0)
        {
            throw Companion::Error::Code::no_cuda_device;
        }

        cv::cuda::GpuMat gpu_scene(sceneImage); // Load scene as an gpu mat
        cv::cuda::GpuMat gpu_object(objectImage); // Load object as an gpu mat
        cv::cuda::GpuMat gpu_descriptors_scene, gpu_descriptors_object;

        this->cudaFeatureMatching->detectAndCompute(gpu_scene, cv::noArray(), keypointsScene, gpu_descriptors_scene);
        this->cudaFeatureMatching->detectAndCompute(gpu_object, cv::noArray(), keypointsObject, gpu_descriptors_object);

        cv::Ptr<cv::cuda::DescriptorMatcher> gpu_matcher = cv::cuda::DescriptorMatcher::createBFMatcher(this->cudaFeatureMatching->defaultNorm());

        gpu_matcher->knnMatch(gpu_descriptors_object, gpu_descriptors_scene, matches, 2);

        gpu_scene.release();
        gpu_object.release();

        // Ratio test for good matches - http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
        // Neighbourhoods comparison
        ratioTest(matches, goodMatches, DEFAULT_RATIO_VALUE);

        drawable = obtainMatchingResult(sceneImage,
            objectImage,
            goodMatches,
            keypointsObject,
            keypointsScene,
            sceneModel,
            objectModel,
            isIRAUsed,
            isROIUsed,
            roi);
    }
    #endif
    else
    {
        // If results are not good enough and empty for keypoints and descriptors.
        sceneImage.release();
        objectImage.release();
        repeatAlgorithm(sceneModel, objectModel, roi, isIRAUsed, ira, isROIUsed);
    }

    if (drawable != nullptr)
    {
        // TODO := SCORING CALCULATION
        // Object found
        result = new Companion::Model::Result(100, objectModel->getID(), drawable);

        sceneImage.release();
        objectImage.release();
    }
    else if (isIRAUsed || isROIUsed)
    {
        // No results and IRA or ROI was used.
        sceneImage.release();
        objectImage.release();
        repeatAlgorithm(sceneModel, objectModel, roi, isIRAUsed, ira, isROIUsed);
    }

    return result;
}

bool Companion::Algorithm::FeatureMatching::isCuda()
{
    return this->cudaUsed;
}

void Companion::Algorithm::FeatureMatching::calculateKeyPoints(Model::Processing::FeatureMatchingModel *model)
{
    if (!isCuda())
    {
        model->calculateKeyPointsAndDescriptors(this->detector, this->extractor);
    }
}

Companion::Model::Result* Companion::Algorithm::FeatureMatching::repeatAlgorithm(
    Model::Processing::FeatureMatchingModel *sceneModel,
    Model::Processing::FeatureMatchingModel *objectModel,
    Companion::Draw::Frame *roi,
    bool isIRAUsed,
    Companion::Algorithm::IRA* ira,
    bool isROIUsed)
{
    if (isIRAUsed)
    {
        ira->clear(); // Clear last detected object position.
        return executeAlgorithm(sceneModel, objectModel, roi); // Repeat algorithm and full scene or roi.
    }
    else if (isROIUsed)
    {
        return executeAlgorithm(sceneModel, objectModel, nullptr); // Repeat algorithm and check full scene.
    }

    return nullptr;
}

void Companion::Algorithm::FeatureMatching::ratioTest(const std::vector<std::vector<cv::DMatch>> &matches,
    std::vector<cv::DMatch> &good_matches,
    float ratio)
{
    unsigned long position = 0;
    bool insertElement;
    for (int i = 0; i < matches.size(); ++i)
    {
        if (matches[i].size() >= 2 && (matches[i][0].distance < ratio * matches[i][1].distance))
        {
            position = 0;
            insertElement = false;
            if(good_matches.empty()) {
                good_matches.push_back(matches[i][0]);
            } else {
                while(position < good_matches.size() && !insertElement) {
                    if(good_matches.at(position).distance > matches[i][0].distance) {
                        good_matches.insert(good_matches.begin() + position, matches[i][0]);
                        insertElement = true;
                        if(good_matches.size() > this->countMatches) {
                            // Delete last element if maximum is reached
                            good_matches.erase(good_matches.end() - 1);
                        }
                    }
                    position++;
                }
                if(!insertElement && good_matches.size() < this->countMatches) {
                    good_matches.push_back(matches[i][0]);
                }
            }

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
    for (int i = 0; i < good_matches.size(); i++)
    {
        trainIdx = good_matches[i].trainIdx;
        queryIdx = good_matches[i].queryIdx;

        if ((trainIdx > 0 && keypoints_scene.size() > trainIdx) && (queryIdx > 0 && keypoints_object.size() > queryIdx))
        {
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
    Model::Processing::FeatureMatchingModel *cModel,
    bool isIRAUsed,
    bool isROIUsed,
    Companion::Draw::Frame *roi)
{

    Companion::Draw::Drawable *drawable = nullptr;
    cv::Mat homography;
    std::vector<cv::Point2f> feature_points_object, feature_points_scene;

    feature_points_object.clear();
    feature_points_scene.clear();

    // Count of good matches if results are good enough.
    if (good_matches.size() >= this->countMatches)
    {

        obtainKeypointsFromGoodMatches(good_matches,
            keypoints_object,
            keypoints_scene,
            feature_points_object,
            feature_points_scene);

        // Find Homography if only features points are filled.
        if (!feature_points_object.empty() && !feature_points_scene.empty())
        {
            homography = cv::findHomography(feature_points_object,
                feature_points_scene,
                this->findHomographyMethod,
                this->reprojThreshold,
                cv::noArray(),
                this->ransacMaxIters);

            if (!homography.empty())
            {
                drawable = calculateArea(homography, sceneImage, objectImage, sModel, cModel, isIRAUsed, isROIUsed, roi);
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
    Model::Processing::FeatureMatchingModel *cModel,
    bool isIRAUsed,
    bool isROIUsed,
    Companion::Draw::Frame *roi) {

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
    cv::Rect lastRect = cv::Rect();
    IRA *ira = cModel->getIra();

    if (isIRAUsed) // IRA was used
    {
        lastRect = ira->getLastObjectPosition();
    }
    else if (isROIUsed)
    {
        lastRect = cv::Rect(roi->getTopLeft(), roi->getBottomRight());
    }

    // Offset is recalculate position from last recognition if exists.
    cv::Point2f offset = cv::Point2f(lastRect.x, lastRect.y);

    // ToDo = What happens if your scan a image upside down?
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
    float maxX = 0.0f;
    float minY = sceneImage.rows;
    float maxY = 0.0f;

    for (cv::Point2f point : scene_corners)
    {
        if (point.x < minX) { minX = point.x; }
        if (point.x > maxX) { maxX = point.x; }
        if (point.y < minY) { minY = point.y; }
        if (point.y > maxY) { maxY = point.y; }
    }

    float width = maxX - minX;
    float height = maxY - minY;

    cv::Point2f scale = cv::Point2f(width / 2.0f, height / 2.0f);
    cv::Point2f start = cv::Point2f(minX, minY) + offset - scale;
    cv::Point2f end = cv::Point2f(maxX, maxY) + offset + scale;

    // Object area.
    cv::Point2f topLeft = scene_corners[0] + offset;
    cv::Point2f topRight = scene_corners[1] + offset;
    cv::Point2f bottomRight = scene_corners[2] + offset;
    cv::Point2f bottomLeft = scene_corners[3] + offset;

    // Validate the rectangular shape of the detected area
    if (Companion::Util::validateShape(topRight, bottomLeft, topLeft, bottomRight, this->minSidelLength))
    {
        // Create a drawable frame to represent the calculated area
        frame = new Companion::Draw::Frame(topLeft, topRight, bottomLeft, bottomRight);
    }

    // If IRA is used...
    if (this->useIRA && frame != nullptr)
    {

        // IRA algo stores position from detected object.
        ira->setLastObjectPosition(start.x, start.y, end.x - start.x, end.y - start.y);
        const cv::Rect &lastObjectPosition = ira->getLastObjectPosition();

        // Check if start point is set correctly
        if (lastObjectPosition.x < 0)
        {
            ira->setX(0);
        }

        // Check if width is not oversized
        if (lastObjectPosition.width + lastObjectPosition.x > originalImg.cols)
        {
            ira->setWidth(originalImg.cols - lastObjectPosition.x);
        }

        if (lastObjectPosition.y < 0) {
            ira->setY(0);
        }

        if (lastObjectPosition.height + lastObjectPosition.y > originalImg.rows)
        {
            ira->setHeight(originalImg.rows - lastObjectPosition.y);
        }

        if (lastObjectPosition.area() <= 0)
        {
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
    std::vector<cv::DMatch>& goodMatches, std::string windowName)
{
    // Build only in debug mode for development
    cv::Mat imgMatches;

    if (!objectImg.empty() && !sceneImg.empty())
    {
        cv::drawMatches(objectImg, objectKeypoints, sceneImg, sceneKeypoints,
            goodMatches, imgMatches, cv::Scalar::all(-1), cv::Scalar::all(-1),
            std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

        //-- Show detected matches
        cv::imshow(windowName, imgMatches);
    }
}
#endif