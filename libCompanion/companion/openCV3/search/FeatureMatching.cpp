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

Comparison *FeatureMatching::algo(Comparison *searchModel, Comparison *compareModel) {

    cv::Mat sceneImage = searchModel->getImage();
    cv::Mat objectImage = compareModel->getImage();

    // Variables
    std::vector<std::vector<cv::DMatch>> matches_one;
    std::vector<cv::DMatch> good_matches;
    std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
    std::vector<cv::Point2f> obj, scene;
    cv::Mat descriptors_object, descriptors_scene;

    // Check if images are loaded
    //if (!Util::is_image_loaded(object_img) || !Util::is_image_loaded(scene)) {
    //    throw CompanionError::error_code::image_not_found;
    //}

    if (compareModel->isLastPositionSet()) {
        sceneImage = cv::Mat(sceneImage, compareModel->getLastPosition());
        cv::imshow("Cut_Image", sceneImage);
    }

    // Step 1 : Detect the keypoints
    detector->detect(sceneImage, keypoints_scene);
    searchModel->setKeypoints(keypoints_scene);

    // 2 : Calculate descriptors (feature vectors)
    extractor->compute(sceneImage, keypoints_scene, descriptors_scene);
    searchModel->setDescriptors(descriptors_scene);

    // --- ToDo Method implementation ---
    if(compareModel->getKeypoints().empty()) {
        // Step 1 : Detect the keypoints
        detector->detect(objectImage, keypoints_object);
        searchModel->setKeypoints(keypoints_object);

        // 2 : Calculate descriptors (feature vectors)
        extractor->compute(objectImage, keypoints_object, descriptors_object);
        searchModel->setDescriptors(descriptors_object);
    } else {
        // Step 1 : Keypoints already detected from model
        keypoints_object = compareModel->getKeypoints();
        descriptors_object = compareModel->getDescriptors();
    }

    if (!descriptors_object.empty() && !descriptors_scene.empty()
        && keypoints_object.size() > 0 && keypoints_scene.size() > 0) {

        // Flan based needs CV_32F
        if (matchingType.compare("FlannBased") == 0) {
            descriptors_scene.convertTo(descriptors_scene, CV_32F);
            descriptors_object.convertTo(descriptors_object, CV_32F);
        }

        //-- Step 3: Matching descriptor vectors
        matcher->knnMatch(descriptors_object, descriptors_scene, matches_one, 2);

        // Ratio test for good matches - http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
        // Neighbourhoods comparison
        ratio_test(matches_one, good_matches, 0.80);

        // Symmetric matches
        // ToDo := How does it works?
        //vector<DMatch> symMatches;
        //symmetry_test(good_matches, good_matches_two, symMatches);
        //good_matches = symMatches;

        //-- Localize the object
        if (good_matches.size() > 40) {

//            std::cout << good_matches.size() << "\n";

            for (int i = 0; i < good_matches.size(); i++) {
                //-- Get the keypoints from the good matches
                obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
                scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
            }

            // Find Homography
            cv::Mat H = cv::findHomography(obj, scene, CV_RANSAC);

            if (!H.empty()) {
                //-- Get the corners from the image_1 ( the object to be "detected" )
                std::vector<cv::Point2f> obj_corners(4);
                obj_corners[0] = cvPoint(0, 0);
                obj_corners[1] = cvPoint(objectImage.cols, 0);
                obj_corners[2] = cvPoint(objectImage.cols, objectImage.rows);
                obj_corners[3] = cvPoint(0, objectImage.rows);
                std::vector<cv::Point2f> scene_corners(4);

                cv::perspectiveTransform(obj_corners, scene_corners, H);

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

                // ToDo:= Lines from position will be returned...
                Lines *lines = new Lines();
                lines->addLine(new Line(scene_corners[0] + offset, scene_corners[1] + offset, color, thickness));
                lines->addLine(new Line(scene_corners[3] + offset, scene_corners[0] + offset, color, thickness));
                lines->addLine(new Line(scene_corners[1] + offset, scene_corners[2] + offset, color, thickness));
                lines->addLine(new Line(scene_corners[2] + offset, scene_corners[3] + offset, color, thickness));
                lines->draw(sceneImage);

                compareModel->setLastPosition(start.x, start.y, end.x - start.x, end.y - start.y);

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

                //std::cout << "y:= " << subImage.y << " x:= " << subImage.x << " width:= " << subImage.width << " height:= " << subImage.height << "\n";
                cv::imshow("Good Matches & Object detection", sceneImage);
            }
        } else {
            // If no match found and image was cutted.
            if(sceneImage.cols != searchModel->getImage().cols || sceneImage.rows != searchModel->getImage().rows) {
                compareModel->setLastPosition(0, 0, 0, 0);
                algo(searchModel, compareModel);
                cv::imshow("Good Matches & Object detection", searchModel->getImage());
            } else {
                cv::imshow("Good Matches & Object detection", sceneImage);
            }
        }

    }

    return nullptr;
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