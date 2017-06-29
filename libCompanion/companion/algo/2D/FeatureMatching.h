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

#include <companion/algo/abstract/AbstractFeatureMatching.h>
#include <companion/draw/Frame.h>
#include <companion/model/result/Result.h>
#include <companion/util/CompanionError.h>

namespace Companion {

    namespace Algorithm {

        /**
         * Feature matching algorithm implementation based on <a href="http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html">OpenCV</a>.
         * @author Andreas Sekulski
         */
        class FeatureMatching : public AbstractFeatureMatching {

        public:

            /**
             * Constructor to create an CPU feature matching algorithm.
             *
             * Following feature matching can be used for example.
             * Detector   : BRISK, ORB, MSER, FastFeatureDetector, AgastFeatureDetector, GFTTDetector, SimpleBlobDetector, KAZE, AKAZE.
             * Extractor  : BRISK, ORB, KAZE, AKAZE.
             * Matcher    : FLANNBASED, BRUTEFORCE, BRUTEFORCE_L1, BruteForce-Hamming, BRUTEFORCE_HAMMINGLUT, BRUTEFORCE_SL2.
             *
             * If you want to use SIFT: detector + descriptor and  SURF: detector + descriptor you must build
             * this lib with XFeatures2D support.
             *
             * @param detector FeatureDetector to set.
             * @param extractor FeatureExtractor to set.
             * @param matcher FeatureMatcher to set.
             * @param matcherType FeatureMatcher type which is used like FlannBased or Bruteforce.
             * @param cornerDistance How many pixels the corners of a found area should be distant from each other. Default value is 10.
             * @param countMatches How much matches need to get an good matching result. Default is by 40.
             * @param useIRA Indicator to use IRA algorithm to use last detected objects from last scene. By default IRA is deactivated.
             */
            FeatureMatching(cv::Ptr<cv::FeatureDetector> detector,
                            cv::Ptr<cv::DescriptorExtractor> extractor,
                            cv::Ptr<cv::DescriptorMatcher> matcher,
                            int matcherType,
                            int cornerDistance = 10,
                            int countMatches = 40,
                            bool useIRA = false);

            // Only build if cuda should be used.
            #if Companion_USE_CUDA
            /**
             * Constructor to create an cuda based feature matching.
             *
             * Following feature matching cuda algorithms can be used cv::cuda::ORB or cv::cuda::SURF.
             *
             * @param cudaFeatureMatching Cuda based feature matching algorithm.
             * @param cornerDistance How many pixels the corners of a found area should be distant from each other. Default value is 10.
             * @param countMatches How much matches need to get an good matching result. Default is by 40.
             */
            FeatureMatching(cv::Ptr<cv::Feature2D> cudaFeatureMatching,
                            int cornerDistance = 10,
                            int countMatches = 40);
            #endif

            #if Companion_DEBUG
            void showFeatureMatches(cv::Mat& objectImg, std::vector<cv::KeyPoint>& objectKeypoints,
                                    cv::Mat& sceneImg, std::vector<cv::KeyPoint>& sceneKeypoints,
                                    std::vector<cv::DMatch>& goodMatches);
            #endif

            /**
             * Default destructor.
             */
            virtual ~FeatureMatching();

            /**
             * Feature matching algorithm implementation by <a href="http://docs.opencv.org/trunk/dc/dc3/tutorial_py_matcher.html">OpenCV</a>.
             * @param sceneModel Scene model from image recognition search, must be from model class <b>FeatureMatchingModel</b>.
             * @param objectModel Object model from image recognition search, must be from model class <b>FeatureMatchingModel</b>.
             * @throws Companion::Error::Code If an error occurred in search operation.
             */
            virtual Companion::Model::Result* algo(Model::Processing::ImageRecognitionModel *sceneModel,
                                                   Model::Processing::ImageRecognitionModel *objectModel);

            /**
             * Indicator if this algorithm use cuda.
             * @return True if cuda will be used otherwise false.
             */
            virtual bool isCuda();

        private:

            /**
              * FeatureMatching type which is used, if is necessary to convert obtained images.
              */
            int matcherType;

            /**
             * Indicator if cuda will be used.
             */
            bool cudaUsed;

            /**
             * FeatureDetector which is used for an cpu usage.
             */
            cv::Ptr<cv::FeatureDetector> detector;

            /**
             * FeatureExtractor which is used for an cpu usage.
             */
            cv::Ptr<cv::DescriptorExtractor> extractor;

            /**
             * FeatureDescriptor which is used for an cpu usage.
             */
            cv::Ptr<cv::DescriptorMatcher> matcher;

            /**
             * Cuda feature matching algorithm.
             */
            cv::Ptr<cv::Feature2D> cudaFeatureMatching;
        };
    }
}

#endif //COMPANION_FEATUREMATCHING_H
