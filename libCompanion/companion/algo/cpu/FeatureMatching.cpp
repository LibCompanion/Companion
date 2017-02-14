#include "FeatureMatching.h"

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

void
FeatureMatching::ratio_test(const std::vector<std::vector<cv::DMatch>> &matches, std::vector<cv::DMatch> &good_matches,
                            float ratio) {
    for (int i = 0; i < matches.size(); ++i) {
        if (matches[i][0].distance < ratio * matches[i][1].distance) {
            good_matches.push_back(matches[i][0]);
        }
    }
}

void FeatureMatching::symmetry_test(const std::vector<cv::DMatch> &matches1, const std::vector<cv::DMatch> &matches2,
                                    std::vector<cv::DMatch> &symMatches) {
    symMatches.clear();

    for (std::vector<cv::DMatch>::const_iterator matchIterator1 = matches1.begin();
         matchIterator1 != matches1.end(); ++matchIterator1) {

        for (std::vector<cv::DMatch>::const_iterator matchIterator2 = matches2.begin();
             matchIterator2 != matches2.end(); ++matchIterator2) {

            if ((*matchIterator1).queryIdx == (*matchIterator2).trainIdx &&
                (*matchIterator2).queryIdx == (*matchIterator1).trainIdx) {
                symMatches.push_back(
                        cv::DMatch((*matchIterator1).queryIdx, (*matchIterator1).trainIdx, (*matchIterator1).distance));
                break;
            }

        }

    }
}

void FeatureMatching::obtainKeypointsFromGoodMatches(std::vector<cv::DMatch> &good_matches,
                                                     std::vector<cv::KeyPoint> &keypoints_object,
                                                     std::vector<cv::KeyPoint> &keypoints_scene,
                                                     std::vector<cv::Point2f> &feature_points_object,
                                                     std::vector<cv::Point2f> &feature_points_scene) {
    // Get the keypoints from the good matches
    for (int i = 0; i < good_matches.size(); i++) {
        feature_points_scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
        feature_points_object.push_back(keypoints_object[good_matches[i].queryIdx].pt);
    }
}

Drawable* FeatureMatching::obtainMatchingResult(cv::Mat &sceneImage,
                                                cv::Mat &objectImage,
                                                std::vector<cv::DMatch> &good_matches,
                                                std::vector<cv::KeyPoint> &keypoints_object,
                                                std::vector<cv::KeyPoint> &keypoints_scene,
                                                FeatureMatchingModel *sModel,
                                                FeatureMatchingModel *cModel) {

    Drawable *lines = nullptr;
    cv::Mat homography;
    std::vector<cv::Point2f> feature_points_object, feature_points_scene;

    // Count of good matches if results are good enough.
    if (good_matches.size() > countMatches) {

        obtainKeypointsFromGoodMatches(good_matches,
                                       keypoints_object,
                                       keypoints_scene,
                                       feature_points_object,
                                       feature_points_scene);

        // Find Homography
        homography = cv::findHomography(feature_points_object, feature_points_scene, CV_RANSAC);

        if (!homography.empty()) {
            lines = calculateArea(homography, sceneImage, objectImage, sModel, cModel);
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
    }

    return lines;
}

Drawable* FeatureMatching::calculateArea(cv::Mat &homography,
                        cv::Mat &sceneImage,
                        cv::Mat &objectImage,
                        FeatureMatchingModel *sModel,
                        FeatureMatchingModel *cModel) {

    //-- Get the corners from the image_1 (the object to be "detected")
    std::vector<cv::Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0, 0);
    obj_corners[1] = cvPoint(objectImage.cols, 0);
    obj_corners[2] = cvPoint(objectImage.cols, objectImage.rows);
    obj_corners[3] = cvPoint(0, objectImage.rows);

    std::vector<cv::Point2f> scene_corners(4);
    cv::perspectiveTransform(obj_corners, scene_corners, homography);

    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    int thickness = 4;
    cv::Scalar color = cv::Scalar(0, 255, 0);

    // IRA
    cv::Rect lastRect = cModel->getLastPosition();
    cv::Point2f offset = cv::Point2f(lastRect.x,
                                     lastRect.y); // Offset is recalculate position from last recognition

    // Focus area - Scene Corners
    //   0               1
    //   -----------------
    //   |               |
    //   |               |
    //   |               |
    //   |               |
    //   -----------------
    //   3               2
    float width = (scene_corners[1] - scene_corners[0]).x;
    float height = (scene_corners[3] - scene_corners[0]).y;
    cv::Point2f scale = cv::Point2f(width / 2, height / 2);
    cv::Point2f start = scene_corners[0] + offset - scale;
    cv::Point2f end = scene_corners[2] + offset + scale;

    // If IRA was used...
    if(sceneImage.cols != sModel->getImage().cols || sceneImage.rows != sModel->getImage().rows) {
        // Restore to original image.
        sceneImage = sModel->getImage();
    }

    // Object area.
    Lines *lines = new Lines();
    lines->addLine(new Line(scene_corners[0] + offset, scene_corners[1] + offset, color, thickness));
    lines->addLine(new Line(scene_corners[3] + offset, scene_corners[0] + offset, color, thickness));
    lines->addLine(new Line(scene_corners[1] + offset, scene_corners[2] + offset, color, thickness));
    lines->addLine(new Line(scene_corners[2] + offset, scene_corners[3] + offset, color, thickness));

    // IRA algo stores position from detected object with an scaling factor.
    cModel->setLastPosition(start.x, start.y, end.x - start.x, end.y - start.y);

    // Check if start point is set correctly
    if (cModel->getLastPosition().x < 0) {
        cModel->setLastPositionX(0);
    }

    // Check if width is not oversized
    if (cModel->getLastPosition().width + cModel->getLastPosition().x > sceneImage.cols) {
        cModel->setLastPositionWidth(sceneImage.cols - cModel->getLastPosition().x);
    }

    if (cModel->getLastPosition().y < 0) {
        cModel->setLastPositionY(0);
    }

    if (cModel->getLastPosition().height + cModel->getLastPosition().y > sceneImage.rows) {
        cModel->setLastPositionHeight(sceneImage.rows - cModel->getLastPosition().y);
    }

    if(cModel->getLastPosition().area() <= 0) {
        // Something goes wrong in area
        cModel->setLastPosition(-1, -1, 0, 0);
    }

    return lines;
}