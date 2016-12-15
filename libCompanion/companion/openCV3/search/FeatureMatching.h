//
// Created by andre on 28.11.2016.
//

#ifndef COMPANION_FEATUREMATCHING_H
#define COMPANION_FEATUREMATCHING_H

#include "ImageRecognition.h"
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/xfeatures2d.hpp>

/**
 * Feature matching algo implementation from <a href="http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html">OpenCV</a>.
 *
 * @author Andreas Sekulski
 */
class FeatureMatching : public ImageRecognition {

public:
    FeatureMatching();

    FeatureMatching(cv::Ptr<cv::FeatureDetector> detector, cv::Ptr<cv::DescriptorExtractor> extractor, cv::Ptr<cv::DescriptorMatcher> matcher, std::string matchingType);

    ~FeatureMatching();

private:

    cv::Rect subImage;

    std::string matchingType;

    cv::Ptr<cv::FeatureDetector> detector;

    cv::Ptr<cv::DescriptorExtractor> extractor;

    cv::Ptr<cv::DescriptorMatcher> matcher;

    void ratio_test(const std::vector<std::vector<cv::DMatch>> &matches, std::vector<cv::DMatch> &good_matches, float ratio);

    void symmetry_test(const std::vector<cv::DMatch> &matches1, const std::vector<cv::DMatch> &matches2, std::vector<cv::DMatch> &symMatches);

protected:
    virtual Comparison *algo(cv::Mat object_img, cv::Mat scene_img);
};


#endif //COMPANION_FEATUREMATCHING_H
