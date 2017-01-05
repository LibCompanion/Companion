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

#ifndef COMPANION_FEATUREMATCHING_H
#define COMPANION_FEATUREMATCHING_H

#include <opencv2/calib3d/calib3d.hpp>
#include "ImageRecognition.h"
#include "companion/openCV3/draw/Lines.h"

/**
 * Feature matching algo implementation based on <a href="http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html">OpenCV</a>.
 * @author Andreas Sekulski
 */
class FeatureMatching : public ImageRecognition {

public:
    /**
     * Default constructor to create feature matching algorithm implementation. Following default settings are used:<br>
     * FeatureDetector = ORB, FeatureExtractor = BRISK, FeatureMatcher = FlannBased
     */
    FeatureMatching();

    /**
     * Constructor to set an specific feature matching algorithms.
     * @param detector FeatureDetector to set.
     * @param extractor FeatureExtractor to set.
     * @param matcher FeatureMatcher to set.
     * @param matchingType FeatureMatcher type which is used like FlannBased or Bruteforce. // ToDo := Enumeration will be better.
     */
    FeatureMatching(cv::Ptr<cv::FeatureDetector> detector, cv::Ptr<cv::DescriptorExtractor> extractor,
                    cv::Ptr<cv::DescriptorMatcher> matcher, std::string matchingType);

    /**
     * Default destructor.
     */
    ~FeatureMatching();

private:

    /**
     * FeatureMatching type which is used, if is necessary to convert obtained images.
     */
    std::string matchingType;

    /**
     * FeatureDetector which is used by default ORB is used.
     */
    cv::Ptr<cv::FeatureDetector> detector;

    /**
     * FeatureExtractor which is used by default BRISK is used.
     */
    cv::Ptr<cv::DescriptorExtractor> extractor;

    /**
     * FeatureDescriptor which is used by default FlannBased is used.
     */
    cv::Ptr<cv::DescriptorMatcher> matcher;

    /**
     * Ratio test implementation to improve results from matching to obtain only good results. <br>
     * Paper -> Neighbourhoods comparison - http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
     * @param matches Matches from feature matching.
     * @param good_matches Vector to store good matches.
     * @param ratio Ratio which matches are good enough.
     */
    void
    ratio_test(const std::vector<std::vector<cv::DMatch>> &matches, std::vector<cv::DMatch> &good_matches, float ratio);

    /**
     * Symmetry test to improve results. Currently not using because i do not understand this algo...
     * @param matches1
     * @param matches2
     * @param symMatches
     */
    void symmetry_test(const std::vector<cv::DMatch> &matches1, const std::vector<cv::DMatch> &matches2,
                       std::vector<cv::DMatch> &symMatches);

    /**
     * Feature matching algorithm implementation by <a href="http://docs.opencv.org/trunk/dc/dc3/tutorial_py_matcher.html">OpenCV</a>.
     * @param searchModel Scene model from image recognition search, must be from model class <b>FeatureMatchingModel</b>.
     * @param compareModel Object model from image recognition search, must be from model class <b>FeatureMatchingModel</b>.
     * @throws CompanionError::error_code If an error occured in search operation.
     */
    virtual Drawable* algo(ImageRecognitionModel *searchModel, ImageRecognitionModel *compareModel);
};

#endif //COMPANION_FEATUREMATCHING_H