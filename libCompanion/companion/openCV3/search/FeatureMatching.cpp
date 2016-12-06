//
// Created by andre on 28.11.2016.
//

#include "FeatureMatching.h"

FeatureMatching::FeatureMatching() {
    this->feature = ORB::create();
    this->descriptor = DescriptorMatcher::create("FlannBased");
}

FeatureMatching::FeatureMatching(Ptr<Feature2D> feature, Ptr<DescriptorMatcher> descriptor) {
    this->feature = feature;
    this->descriptor = descriptor;
}

FeatureMatching::~FeatureMatching() {}

Comparison *FeatureMatching::algo(Mat search_img, Mat compare_img) {

    // Check if images are loaded
    if (!Util::is_image_loaded(search_img) || !Util::is_image_loaded(compare_img)) {
        throw CompanionError::error_code::image_not_found;
    }

    // ToDo Check if feature and descriptor set

    // http://docs.opencv.org/3.1.0/d5/d51/group__features2d__main.html
    // https://stackoverflow.com/questions/36691050/opencv-3-list-of-available-featuredetectorcreate-and-descriptorextractorc
    // http://docs.opencv.org/3.1.0/d7/d66/tutorial_feature_detection.html
    // http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html
    vector<DMatch> matches;
    vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;

    // Step 1 : Detect the keypoints & 2 : Calculate descriptors (feature vectors)
    feature->detectAndCompute(search_img, Mat(), keypoints_object, descriptors_object);
    feature->detectAndCompute(compare_img, Mat(), keypoints_scene, descriptors_scene);

    if (!descriptors_object.empty() && !descriptors_scene.empty() && keypoints_object.size() > 0 &&
        keypoints_scene.size() > 0) {

        // Flan based needs CV_32F ToDo: Check others...
        descriptors_scene.convertTo(descriptors_scene, CV_32F);
        descriptors_object.convertTo(descriptors_object, CV_32F);

        //-- Step 3: Matching descriptor vectors
        descriptor->match(descriptors_object, descriptors_scene, matches, 0);

        //-- Step 4: Result creating for matching
        double max_dist = 0;
        double min_dist = 100;

        //-- Quick calculation of max and min distances between keypoints
        for (int i = 0; i < descriptors_object.rows; i++) {
            double dist = matches[i].distance;
            if (dist < min_dist) {
                min_dist = dist;
            }
            if (dist > max_dist) {
                max_dist = dist;
            }
        }

        //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
        //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
        //-- small)
        //-- PS.- radiusMatch can also be used here.
        vector<DMatch> good_matches;
        double good_matches_sum = 0.0;
        double score = 1;

        for (int i = 0; i < descriptors_object.rows; i++) {
            if (matches[i].distance <= max(2 * min_dist, 0.02)) {
                good_matches.push_back(matches[i]);
                good_matches_sum += matches[i].distance;
            }
        }

        // If score = 0 img is equal > 0 threshold decision
        if (good_matches.size() > 0) {
            score = good_matches_sum / good_matches.size();
            if (score > 1) {
                score = 1;
            } else if (score < 0) {
                score = 0;
            }
        }

        // ToDo get Position from image
        Point matchLoc = Point(0, 0);
        Point matchOffset = Point(compare_img.cols, compare_img.rows);

        //-- Draw only "good" matches
        Mat img_matches = compare_img;
        drawMatches(search_img, keypoints_object, compare_img, keypoints_scene,
                    good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                    vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

        //-- Localize the object
        vector<Point2f> obj;
        vector<Point2f> scene;

        if (good_matches.size() > 0) {

            for (int i = 0; i < good_matches.size(); i++) {
                //-- Get the keypoints from the good matches
                obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
                scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
            }

            // Find Homography
            Mat H = findHomography(obj, scene, CV_RANSAC);

            if (!H.empty()) {
                //-- Get the corners from the image_1 ( the object to be "detected" )
                std::vector<Point2f> obj_corners(4);
                obj_corners[0] = cvPoint(0, 0);
                obj_corners[1] = cvPoint(search_img.cols, 0);
                obj_corners[2] = cvPoint(search_img.cols, search_img.rows);
                obj_corners[3] = cvPoint(0, search_img.rows);
                std::vector<Point2f> scene_corners(4);

                perspectiveTransform(obj_corners, scene_corners, H);

                //-- Draw lines between the corners (the mapped object in the scene - image_2 )
                line(img_matches, scene_corners[0] + Point2f(search_img.cols, 0),
                     scene_corners[1] + Point2f(search_img.cols, 0), Scalar(0, 255, 0), 4);
                line(img_matches, scene_corners[1] + Point2f(search_img.cols, 0),
                     scene_corners[2] + Point2f(search_img.cols, 0), Scalar(0, 255, 0), 4);
                line(img_matches, scene_corners[2] + Point2f(search_img.cols, 0),
                     scene_corners[3] + Point2f(search_img.cols, 0), Scalar(0, 255, 0), 4);
                line(img_matches, scene_corners[3] + Point2f(search_img.cols, 0),
                     scene_corners[0] + Point2f(search_img.cols, 0), Scalar(0, 255, 0), 4);
            }

        }

        //-- Show detected matches
        imshow("Good Matches & Object detection", img_matches);
    }

    return nullptr;

    // ToDo Store Informations
    //return new FeatureMatch(search_img_path, compare_img_path, score, good_matches, keypoints_object, keypoints_scene);
}

const Ptr<Feature2D> &FeatureMatching::getFeature() const {
    return feature;
}

void FeatureMatching::setFeature(const Ptr<Feature2D> &feature) {
    FeatureMatching::feature = feature;
}

const Ptr<DescriptorMatcher> &FeatureMatching::getDescriptor() const {
    return descriptor;
}

void FeatureMatching::setDescriptor(const Ptr<DescriptorMatcher> &descriptor) {
    FeatureMatching::descriptor = descriptor;
}
