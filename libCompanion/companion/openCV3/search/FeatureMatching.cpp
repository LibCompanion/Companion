//
// Created by andre on 28.11.2016.
//

#include "FeatureMatching.h"

FeatureMatching::FeatureMatching() {
    this->detector = FeatureMatcher::detector::KAZE;
    this->matcher = FeatureMatcher::matcher::FlannBased;
}

FeatureMatching::FeatureMatching(FeatureMatcher::detector detector, FeatureMatcher::matcher) {
    this->detector = detector;
    this->matcher = matcher;
}

FeatureMatching::~FeatureMatching() {}

Comparison* FeatureMatching::algo(Mat search_img, Mat compare_img) {

	// Check if images are loaded
	if (!Util::is_image_loaded(search_img) || !Util::is_image_loaded(compare_img)) {
        throw CompanionError::error_code::image_not_found;
	}

    // http://docs.opencv.org/3.1.0/d5/d51/group__features2d__main.html
    // https://stackoverflow.com/questions/36691050/opencv-3-list-of-available-featuredetectorcreate-and-descriptorextractorc
    // http://docs.opencv.org/3.1.0/d7/d66/tutorial_feature_detection.html
    // http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html
    // ToDo := All feature detectors used?
    vector<DMatch> matches;
    vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;

    // Step 1 : Detect the keypoints & 2 : Calculate descriptors (feature vectors)
    Ptr<Feature2D> feature = get_feature_detector(detector);
    feature->detectAndCompute(search_img, Mat(), keypoints_object, descriptors_object);
    feature->detectAndCompute(compare_img, Mat(), keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors
    Ptr<DescriptorMatcher> descriptor_matcher = DescriptorMatcher::create(get_decriptor_matcher(matcher));
    descriptor_matcher->match(descriptors_object, descriptors_scene, matches, 0);

	//-- Step 4: Result creating for matching
	double max_dist = 0;
	double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist)
		{
			min_dist = dist;
		}
		if (dist > max_dist)
		{
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
		if (matches[i].distance <= max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
			good_matches_sum += matches[i].distance;
		}
	}

	// If score = 0 img is equal > 0 threshold decision
	if (good_matches.size() > 0)
	{
		score = good_matches_sum / good_matches.size();
		if (score > 1)
		{
			score = 1;
		}
		else if (score < 0)
		{
			score = 0;
		}
	}

    // ToDo get Position from image
    Point matchLoc = Point(0, 0);
    Point matchOffset = Point(compare_img.cols, compare_img.rows);

    return new Comparison(score, matchLoc, matchOffset);

    // ToDo Store Informations
    //return new FeatureMatch(search_img_path, compare_img_path, score, good_matches, keypoints_object, keypoints_scene);
}

Ptr<Feature2D> FeatureMatching::get_feature_detector(FeatureMatcher::detector f_detector)
{
    Ptr<Feature2D> feature;

    switch (f_detector)
    {
        case FeatureMatcher::detector::AKAZE:
            feature = AKAZE::create();
            break;
        case FeatureMatcher::detector::BRISK:
            feature = BRISK::create();
            break;
        case FeatureMatcher::detector::FastFeatureDetector:
            feature = FastFeatureDetector::create();
            break;
        case FeatureMatcher::detector::GFTTDetector:
            feature = GFTTDetector::create();
            break;
        case FeatureMatcher::detector::KAZE:
            feature = KAZE::create();
            break;
        case FeatureMatcher::detector::MSER:
            feature = MSER::create();
            break;
        case FeatureMatcher::detector::ORB:
            feature = ORB::create();
            break;
        case FeatureMatcher::detector::SimpleBlobDetector:
            feature = SimpleBlobDetector::create();
            break;
    }

    return feature;
}

string FeatureMatching::get_decriptor_matcher(FeatureMatcher::matcher f_matcher) {
    string descriptor_matcher_type;

    switch (f_matcher)
    {
        case FeatureMatcher::matcher::BruteForce_L2:
            descriptor_matcher_type = "BruteForce";
            break;
        case FeatureMatcher::matcher::BruteForce_L1:
            descriptor_matcher_type = "BruteForce-L1";
            break;
        case FeatureMatcher::matcher::BruteForce_Hamming:
            descriptor_matcher_type = "BruteForce-Hamming";
            break;
        case FeatureMatcher::matcher::BruteForce_Hamming_2:
            descriptor_matcher_type = "BruteForce-Hamming(2)";
            break;
        case FeatureMatcher::matcher::FlannBased:
            descriptor_matcher_type = "FlannBased";
            break;
    }

    return descriptor_matcher_type;
}