#include "FeatureMatching.h"

FeatureMatching::FeatureMatching() {

    this->detector = cv::ORB::create();
    this->extractor = cv::BRISK::create();
    this->matchingType = "FlannBased";
    this->matcher = cv::DescriptorMatcher::create(matchingType);
}

FeatureMatching::FeatureMatching(cv::Ptr<cv::FeatureDetector> detector, cv::Ptr<cv::DescriptorExtractor> extractor,
                                 cv::Ptr<cv::DescriptorMatcher> matcher, std::string matchingType) {
    this->detector = detector;
    this->extractor = extractor;
    this->matcher = matcher;
    this->matchingType = matchingType;
}

FeatureMatching::~FeatureMatching() {}

Drawable *FeatureMatching::algo(ImageRecognitionModel *searchModel, ImageRecognitionModel *compareModel) {

    // Set of variables for feature matching.
    cv::Mat homography;
    cv::Mat sceneImage;
    cv::Mat objectImage;
    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> good_matches;
    std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
    std::vector<cv::Point2f> feature_points_object, feature_points_scene;
    cv::Mat descriptors_object, descriptors_scene;
    Lines *lines = nullptr;

    FeatureMatchingModel *smodel = dynamic_cast<FeatureMatchingModel *>(searchModel);
    FeatureMatchingModel *cmodel = dynamic_cast<FeatureMatchingModel *>(compareModel);

    // If wrong model types are used...
    if(!smodel || !cmodel) {
        throw CompanionError::errorCode::wrong_model_type;
    }

    sceneImage = smodel->getImage();
    objectImage = cmodel->getImage();

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
        if(smodel->getKeypoints().empty()) {
            // Step 1 : Detect the keypoints
            detector->detect(sceneImage, keypoints_scene);
            smodel->setKeypoints(keypoints_scene);
            // Step 2 : Calculate descriptors (feature vectors)
            extractor->compute(sceneImage, keypoints_scene, descriptors_scene);
            smodel->setDescriptors(descriptors_scene);
        } else {
            // Keypoints and descriptors already calculated from model
            keypoints_scene = smodel->getKeypoints();
            descriptors_scene = smodel->getDescriptors();
        }
    }

    // --- ToDo Method implementation ---
    // If model has already keypoints calculated from object...
    if(cmodel->getKeypoints().empty()) {
        // Step 1 : Detect the keypoints
        detector->detect(objectImage, keypoints_object);
        cmodel->setKeypoints(keypoints_object);
        // Step 2 : Calculate descriptors (feature vectors)
        extractor->compute(objectImage, keypoints_object, descriptors_object);
        cmodel->setDescriptors(descriptors_object);
    } else {
        // Keypoints and descriptors already calculated from model
        keypoints_object = cmodel->getKeypoints();
        descriptors_object = cmodel->getDescriptors();
    }

    if (!descriptors_object.empty() && !descriptors_scene.empty()
        && keypoints_object.size() > 0 && keypoints_scene.size() > 0) {

        // If matching type is flan based, scene and object must be in CV_32F format.
        if (matchingType.compare("FlannBased") == 0) {
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

        // Count of good matches if results are good enough.
        if (good_matches.size() > 40) {

            // Get the keypoints from the good matches
            for (int i = 0; i < good_matches.size(); i++) {
                feature_points_object.push_back(keypoints_object[good_matches[i].queryIdx].pt);
                feature_points_scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
            }

            // Find Homography
            homography = cv::findHomography(feature_points_object, feature_points_scene, CV_RANSAC);

            if (!homography.empty()) {
                //-- Get the corners from the image_1 ( the object to be "detected" )
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
                cv::Rect lastRect = compareModel->getLastPosition();
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

                if(sceneImage.cols != searchModel->getImage().cols || sceneImage.rows != searchModel->getImage().rows) {
                    // Restore to original image.
                    sceneImage = searchModel->getImage();
                }

                lines = new Lines();
                lines->addLine(new Line(scene_corners[0] + offset, scene_corners[1] + offset, color, thickness));
                lines->addLine(new Line(scene_corners[3] + offset, scene_corners[0] + offset, color, thickness));
                lines->addLine(new Line(scene_corners[1] + offset, scene_corners[2] + offset, color, thickness));
                lines->addLine(new Line(scene_corners[2] + offset, scene_corners[3] + offset, color, thickness));

                cmodel->setLastPosition(start.x, start.y, end.x - start.x, end.y - start.y);

                int valid_x = sceneImage.cols - (compareModel->getLastPosition().x + compareModel->getLastPosition().width);
                int valid_y = sceneImage.rows - (compareModel->getLastPosition().y + compareModel->getLastPosition().height);

                if (valid_x < 0) {
                    compareModel->setLastPositionWidth(compareModel->getLastPosition().width + valid_x);
                } else if (compareModel->getLastPosition().x < 0) {
                    compareModel->setLastPositionX(0);
                }

                if (valid_y < 0) {
                    compareModel->setLastPositionHeight(compareModel->getLastPosition().height + valid_y);
                } else if (compareModel->getLastPosition().y < 0) {
                    compareModel->setLastPositionY(0);
                }

                if (compareModel->getLastPosition().width < 0 || compareModel->getLastPosition().height < 0) {
                    compareModel->setLastPosition(0, 0, 0, 0);
                }
            }
        } else {
            // If result is not good enough and last image was cutted from scene.
            if(sceneImage.cols != searchModel->getImage().cols || sceneImage.rows != searchModel->getImage().rows) {
                compareModel->setLastPosition(0, 0, 0, 0); // Reset position because object is no more detected...
                return algo(searchModel, compareModel);
            }
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