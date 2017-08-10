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

#include <companion/algo/abstract/Matching.h>
#include <companion/draw/Frame.h>
#include <companion/model/result/Result.h>
#include <companion/util/CompanionError.h>

namespace Companion {

    namespace Algorithm {

        /**
         * Feature matching algorithm implementation based on <a href="http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html">OpenCV</a>.
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS FeatureMatching : public Matching {

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
             * @param reprojThreshold Homography parameter: Maximum allowed reprojection error to treat a point pair as an inlier. Default is by 3.0.
             * @param ransacMaxIters Homography parameter: The maximum number of RANSAC iterations, 2000 is the maximum it can be. Default is by 500.
             * @param findHomographyMethod Method used to computed a homography matrix. Default is by RANSAC.
             */
            FeatureMatching(cv::Ptr<cv::FeatureDetector> detector,
                            cv::Ptr<cv::DescriptorExtractor> extractor,
                            cv::Ptr<cv::DescriptorMatcher> matcher,
                            int matcherType,
                            int cornerDistance = 10,
                            int countMatches = 40,
                            bool useIRA = false,
                            double reprojThreshold = 3.0,
                            int ransacMaxIters = 500,
                            int findHomographyMethod = cv::RANSAC);

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
             * @param reprojThreshold Homography parameter: Maximum allowed reprojection error to treat a point pair as an inlier. Default is by 3.0.
             * @param ransacMaxIters Homography parameter: The maximum number of RANSAC iterations, 2000 is the maximum it can be. Default is by 500.
             * @param findHomographyMethod Method used to computed a homography matrix. Default is by RANSAC.
             */
            FeatureMatching(cv::Ptr<cv::Feature2D> cudaFeatureMatching,
                            int cornerDistance = 10,
                            int countMatches = 40,
                            double reprojThreshold = 3.0,
                            int ransacMaxIters = 500,
                            int findHomographyMethod = cv::RANSAC);
            #endif

            #if Companion_DEBUG
            void showFeatureMatches(cv::Mat& objectImg, std::vector<cv::KeyPoint>& objectKeypoints,
                                    cv::Mat& sceneImg, std::vector<cv::KeyPoint>& sceneKeypoints,
                                    std::vector<cv::DMatch>& goodMatches, std::string windowName);
            #endif

            /**
             * Default destructor.
             */
            virtual ~FeatureMatching();

            /**
             * Ratio test implementation to improve results from matching to obtain only good results. <br>
             * Paper -> Neighbourhoods comparison - <a href="http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20"> Paper </a>
             * @param matches Matches from feature matching.
             * @param good_matches Vector to store good matches.
             * @param ratio Ratio which matches are good enough.
             */
            void ratio_test(const std::vector<std::vector<cv::DMatch>> &matches,
                            std::vector<cv::DMatch> &good_matches,
                            float ratio);

            /**
             * Filter to obtain only good feature point which matches.
             * @param good_matches Good matches to store.
             * @param keypoints_object Keypoints from object.
             * @param keypoints_scene Keypoints from scene.
             * @param feature_points_object Feature points from object.
             * @param feature_points_scene Feature points from scene.
             */
            void obtainKeypointsFromGoodMatches(std::vector<cv::DMatch> &good_matches,
                                                std::vector<cv::KeyPoint> &keypoints_object,
                                                std::vector<cv::KeyPoint> &keypoints_scene,
                                                std::vector<cv::Point2f> &feature_points_object,
                                                std::vector<cv::Point2f> &feature_points_scene);

            /**
             * Calculates area position from detected object in scene.
             * @param homography Homography to find objects position.
             * @param sceneImage Scene image to detec object image.
             * @param objectImage Object image to detect in scene.
             * @param sModel Feature matching model from scene.
             * @param cModel Feature matching model frome object.
             * @return An drawable class which contains positionion from object in scene image.
             */
            Draw::Drawable* calculateArea(cv::Mat &homography,
                                          cv::Mat &sceneImage,
                                          cv::Mat &objectImage,
                                          Model::Processing::FeatureMatchingModel *sModel,
                                          Model::Processing::FeatureMatchingModel *cModel);

            /**
             * Obtain from given feature matching an result if object was detecet in image or not.
             * @param sceneImage Scene image to detect object.
             * @param objectImage Object image to detect in scene.
             * @param good_matches Vector which contains good matches from object and scene.
             * @param keypoints_object Keypoints from object.
             * @param keypoints_scene Keypoints from scene.
             * @param sModel Scene feature matching model.
             * @param cModel Object feature matching model.
             * @return <b>nullptr</b> if object is not in scene detected otherwise an Drawable class which contains
             *         position from detected objekt.
             */
            Draw::Drawable* obtainMatchingResult(cv::Mat &sceneImage,
                                                 cv::Mat &objectImage,
                                                 std::vector<cv::DMatch> &good_matches,
                                                 std::vector<cv::KeyPoint> &keypoints_object,
                                                 std::vector<cv::KeyPoint> &keypoints_scene,
                                                 Model::Processing::FeatureMatchingModel *sModel,
                                                 Model::Processing::FeatureMatchingModel *cModel);

            /**
             * Specific algorithm implementation like simple compare or template matching.
             * @throws Companion::Error::Code If an error occurred in search operation.
             */
            Companion::Model::Result* executeAlgorithm();

            /**
             * Indicator if this algorithm use cuda.
             * @return True if cuda will be used otherwise false.
             */
            bool isCuda();

        private:

            /**
             * Indicator how many pixels the corners of a found area should be distant from each other. Default value is 10.
             */
            int cornerDistance = 10;

            /**
             * Indicator how much matches need to get an good matching result. Default value is 40.
             */
            int countMatches = 40;

            /**
             * Indicator to use IRA algorithm.
             */
            bool useIRA = false;

            /**
             * Homography parameter: Method used to computed a homography matrix. The following methods are possible:
             *      - 0      (a regular method using all the points)
             *      - RANSAC (RANSAC-based robust method)
             *      - LMEDS  (Least-Median robust method)
             *      - RHO    (PROSAC-based robust method)
             * Default value is RANSAC.
             */
            int findHomographyMethod = cv::RANSAC;

            /**
             * Homography parameter: Maximum allowed reprojection error to treat a point pair as an inlier (used in the RANSAC and RHO methods only).
             * Default value is 3.0.
             */
            double reprojThreshold = 3.0;

            /**
             * Homography parameter: The maximum number of RANSAC iterations, 2000 is the maximum it can be. Default value is 500.
             * ToDo: Performance optimization: http://code.opencv.org/issues/1544
             */
            int ransacMaxIters = 500;

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
