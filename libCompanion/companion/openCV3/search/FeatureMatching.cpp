//
// Created by andre on 28.11.2016.
//

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

Comparison *FeatureMatching::algo(cv::Mat object_img, cv::Mat scene_img) {

    // Check if images are loaded
    if (!Util::is_image_loaded(object_img) || !Util::is_image_loaded(scene_img)) {
        throw CompanionError::error_code::image_not_found;
    }

    // Variables
    std::vector<std::vector<cv::DMatch>> matches_one;
    std::vector<cv::DMatch> good_matches;
    std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
    std::vector<cv::Point2f> obj, scene;
    cv::Mat descriptors_object, descriptors_scene;

    // Step 1 : Detect the keypoints
    detector->detect(object_img, keypoints_object);
    detector->detect(scene_img, keypoints_scene);

    // 2 : Calculate descriptors (feature vectors)
    extractor->compute(object_img, keypoints_object, descriptors_object);
    extractor->compute(scene_img, keypoints_scene, descriptors_scene);

    if (!descriptors_object.empty() && !descriptors_scene.empty()
        && keypoints_object.size() > 0 && keypoints_scene.size() > 0) {

        // Flan based needs CV_32F
        if(matchingType.compare("FlannBased") == 0) {
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

        //-- Draw only "good" matches
        cv::Mat img_matches = scene_img;
        //drawMatches(object_img, keypoints_object, scene_img, keypoints_scene,
        //            good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
        //            std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

        //-- Localize the object
        if (good_matches.size() > 40) {

            std::cout << good_matches.size() << "\n";

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
                obj_corners[1] = cvPoint(object_img.cols, 0);
                obj_corners[2] = cvPoint(object_img.cols, object_img.rows);
                obj_corners[3] = cvPoint(0, object_img.rows);
                std::vector<cv::Point2f> scene_corners(4);

                cv::perspectiveTransform(obj_corners, scene_corners, H);

                //-- Draw lines between the corners (the mapped object in the scene - image_2 )
                int thickness = 4;
                cv::Scalar color = cv::Scalar(0, 255, 0);
                cv::Point2f offset = cv::Point2f(0, 0); // Original offset from default window is zero
                //cv::Point2f offset = cv::Point2f(object_img.cols, 0); // Offset if image draw will be used

                cv::line(img_matches, scene_corners[0] + offset, scene_corners[1] + offset, color, thickness);
                cv::line(img_matches, scene_corners[1] + offset, scene_corners[2] + offset, color, thickness);
                cv::line(img_matches, scene_corners[2] + offset, scene_corners[3] + offset, color, thickness);
                cv::line(img_matches, scene_corners[3] + offset, scene_corners[0] + offset, color, thickness);
            }

        }

        //-- Show detected matches
        cv::imshow("Good Matches & Object detection", img_matches);
    }

    return nullptr;

    // ToDo Store Informations
    //return new FeatureMatch(search_img_path, compare_img_path, score, good_matches, keypoints_object, keypoints_scene);
}

void FeatureMatching::ratio_test(const std::vector<std::vector<cv::DMatch>> &matches, std::vector<cv::DMatch> &good_matches, float ratio) {
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