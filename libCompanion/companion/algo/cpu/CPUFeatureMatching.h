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

#ifndef COMPANION_CPUFEATUREMATCHING_H
#define COMPANION_CPUFEATUREMATCHING_H

#include <opencv2/calib3d/calib3d.hpp>

#include "companion/algo/abstract/AbstractFeatureMatching.h"
#include "companion/draw/Lines.h"

/**
 * Feature matching algo implementation based on <a href="http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html">OpenCV</a>.
 * @author Andreas Sekulski
 */
class CPUFeatureMatching : public AbstractFeatureMatching {

public:

    /**
     * Default constructor to create feature matching algorithm implementation. Following default settings are used:<br>
     * FeatureDetector = ORB, FeatureExtractor = BRISK, FeatureMatcher = FlannBased
     */
    CPUFeatureMatching();

    /**
     *
     * Constructor to set an specific feature matching algorithms.
     *
     * Following feature matching can be used for example.
     * Detector   : BRISK, ORB, MSER, FastFeatureDetector, AgastFeatureDetector, GFTTDetector, SimpleBlobDetector, KAZE, AKAZE.
     * Extractor  : BRISK, ORB, KAZE, AKAZE.
     * Matcher    : FLANNBASED, BRUTEFORCE, BRUTEFORCE_L1, BruteForce-Hamming, BRUTEFORCE_HAMMINGLUT, BRUTEFORCE_SL2.
     *
     * If you want to use SIFT: detector + descriptor and  SURF: detector + descriptor you must build this lib with
     * XFeatures2D support.
     *
     * @param detector FeatureDetector to set.
     * @param extractor FeatureExtractor to set.
     * @param matcher FeatureMatcher to set.
     * @param type FeatureMatcher type which is used like FlannBased or Bruteforce.
     * @param countMatches How much matches need to get an good matching result. Default is by 40.
     */
    CPUFeatureMatching(cv::Ptr<cv::FeatureDetector> detector, cv::Ptr<cv::DescriptorExtractor> extractor,
                    cv::Ptr<cv::DescriptorMatcher> matcher, int type, int countMatches = 40);

    /**
     * Default destructor.
     */
    virtual ~CPUFeatureMatching();

    /**
     * Feature matching algorithm implementation by <a href="http://docs.opencv.org/trunk/dc/dc3/tutorial_py_matcher.html">OpenCV</a>.
     * @param searchModel Scene model from image recognition search, must be from model class <b>FeatureMatchingModel</b>.
     * @param compareModel Object model from image recognition search, must be from model class <b>FeatureMatchingModel</b>.
     * @throws CompanionError::error_code If an error occured in search operation.
     */
    virtual Drawable* algo(ImageRecognitionModel *searchModel, ImageRecognitionModel *compareModel);

    /**
     * Indicator if this algorithm use cuda.
     * @return True if cuda will be used otherwise false.
     */
    virtual bool isCuda();

private:

    /**
     * FeatureMatching type which is used, if is necessary to convert obtained images.
     */
    int type;

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

};

#endif //COMPANION_CPUFEATUREMATCHING_H