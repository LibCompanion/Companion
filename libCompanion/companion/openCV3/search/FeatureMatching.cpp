//
// Created by andre on 28.11.2016.
//

#include "FeatureMatching.h"

FeatureMatching::FeatureMatching() {

    this->detector = ORB::create();
    this->extractor = BRISK::create();
    this->matcher = DescriptorMatcher::create("FlannBased");
}

FeatureMatching::FeatureMatching(Ptr<FeatureDetector> detector, Ptr<DescriptorExtractor> extractor, Ptr<DescriptorMatcher> matcher) {
    this->detector = detector;
    this->extractor = extractor;
    this->matcher = matcher;
}

FeatureMatching::~FeatureMatching() {}

Comparison *FeatureMatching::algo(Mat object_img, Mat scene_img) {

    // Check if images are loaded
    if (!Util::is_image_loaded(object_img) || !Util::is_image_loaded(scene_img)) {
        throw CompanionError::error_code::image_not_found;
    }

    // Variables
    vector<vector<DMatch>> matches;
    vector<DMatch> good_matches;
    vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;
    double max_dist = 0;
    double min_dist = 100;
    double dist;
    double good_matches_sum = 0.0;
    double score = 1;

    // Step 1 : Detect the keypoints
    detector->detect(object_img, keypoints_object);
    detector->detect(scene_img, keypoints_scene);

    // 2 : Calculate descriptors (feature vectors)
    extractor->compute(object_img, keypoints_object, descriptors_object);
    extractor->compute(scene_img, keypoints_scene, descriptors_scene);

    if (!descriptors_object.empty() && !descriptors_scene.empty()
        && keypoints_object.size() > 0 && keypoints_scene.size() > 0) {

        // Flan based needs CV_32F ToDo: Check others...
        descriptors_scene.convertTo(descriptors_scene, CV_32F);
        descriptors_object.convertTo(descriptors_object, CV_32F);

        //-- Step 3: Matching descriptor vectors
        matcher->knnMatch(descriptors_object, descriptors_scene, matches, 2);

        //-- Step 4: Result creating for matching ToDo := Do I need it
        //-- Quick calculation of max and min distances between keypoints
        /*
        for (int i = 0; i < descriptors_object.rows; i++) {
            dist = matches[i].distance;

            if (dist < min_dist) {
                min_dist = dist;
            }

            if (dist > max_dist) {
                max_dist = dist;
            }
        }
        */

        //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
        //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
        //-- small)
        //-- PS.- radiusMatch can also be used here.
        for (int i = 0; i < matches.size(); ++i) {
            // http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
            // Neighbourhoods comparison <-- Better comparison
            const float ratio = 0.80; // As in Lowe's paper; can be tuned
            if (matches[i][0].distance < ratio * matches[i][1].distance) {
                good_matches.push_back(matches[i][0]);
            }

            /* Old comparison contains many false positives
            cout << matches[i].distance << "\n";
            if (matches[i].distance <= max(2 * min_dist, 0.02)) {
                good_matches.push_back(matches[i]);
                good_matches_sum += matches[i].distance;
            }
             */
        }

        // Calculating scoring
        // If score = 0 img is equal > 0 threshold decision
        /*
        if (good_matches.size() > 0) {
            score = good_matches_sum / good_matches.size();
            if (score > 1) {
                score = 1;
            } else if (score < 0) {
                score = 0;
            }
        }
        */

        //-- Draw only "good" matches
        Mat img_matches = scene_img;
        drawMatches(object_img, keypoints_object, scene_img, keypoints_scene,
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
                obj_corners[1] = cvPoint(object_img.cols, 0);
                obj_corners[2] = cvPoint(object_img.cols, object_img.rows);
                obj_corners[3] = cvPoint(0, object_img.rows);
                std::vector<Point2f> scene_corners(4);

                perspectiveTransform(obj_corners, scene_corners, H);

                //-- Draw lines between the corners (the mapped object in the scene - image_2 )
                int thickness = 4;
                Scalar color = Scalar(0, 255, 0);
                // Point2f offset = Point2f(0, 0); // Original offset from default window is zero
                Point2f offset = Point2f(object_img.cols, 0); // Offset if image draw will be used
                line(img_matches, scene_corners[0] + offset, scene_corners[1] + offset, color, thickness);
                line(img_matches, scene_corners[1] + offset, scene_corners[2] + offset, color, thickness);
                line(img_matches, scene_corners[2] + offset, scene_corners[3] + offset, color, thickness);
                line(img_matches, scene_corners[3] + offset, scene_corners[0] + offset, color, thickness);
            }

        }

        //-- Show detected matches
        imshow("Good Matches & Object detection", img_matches);
    }

    return nullptr;

    // ToDo Store Informations
    //return new FeatureMatch(search_img_path, compare_img_path, score, good_matches, keypoints_object, keypoints_scene);
}
