//
// Created by andre on 28.11.2016.
//

#ifndef COMPANION_FEATUREMATCHING_H
#define COMPANION_FEATUREMATCHING_H

#include "ImageRecognition.h"
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;

/**
 * Feature matching algo implementation from <a href="http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html">OpenCV</a>.
 *
 * @author Andreas Sekulski
 */
class FeatureMatching : public ImageRecognition {

public:
    FeatureMatching();

    FeatureMatching(Ptr<FeatureDetector> detector, Ptr<DescriptorExtractor> extractor, Ptr<DescriptorMatcher> matcher);

    ~FeatureMatching();

private:

    Ptr<FeatureDetector> detector;

    Ptr<DescriptorExtractor> extractor;

    Ptr<DescriptorMatcher> matcher;

    void ratio_test(const vector<vector<DMatch>> &matches, vector<DMatch> &good_matches, float ratio);

    void symmetry_test(const vector<DMatch> &matches1, const vector<DMatch> &matches2, vector<DMatch> &symMatches);

protected:
    virtual Comparison *algo(Mat object_img, Mat scene_img);
};


#endif //COMPANION_FEATUREMATCHING_H
