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

#ifndef COMPANION_ABSTRACTFEATUREMATCHING_H
#define COMPANION_ABSTRACTFEATUREMATCHING_H

#include "ImageRecognition.h"
#include "companion/draw/Lines.h"

namespace Companion {

    namespace Algorithm {

        /**
         * Abstract feature matching class implementation for default feature matching methods.
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS AbstractFeatureMatching : public ImageRecognition {

        public:

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
             * Symmetry test to improve results. Currently not using because i do not understand this algo...
             * @param matches1
             * @param matches2
             * @param symMatches
             */
            void symmetry_test(const std::vector<cv::DMatch> &matches1,
                               const std::vector<cv::DMatch> &matches2,
                               std::vector<cv::DMatch> &symMatches);

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
                                    Model::FeatureMatchingModel *sModel,
                                    Model::FeatureMatchingModel *cModel);

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
                                           Model::FeatureMatchingModel *sModel,
                                           Model::FeatureMatchingModel *cModel);

            /**
             * Specific algorithm implementation like simple compare or template matching.
             * @param searchModel Search model to compare.
             * @param compareModel Compare model to check if contains in search model.
             * @throws Companion::Error::Code If an error occured in search operation.
             */
            virtual Draw::Drawable* algo(Model::ImageRecognitionModel *searchModel,
                                         Model::ImageRecognitionModel *compareModel) = 0;

            /**
             * Indicator if this algorithm use cuda.
             * @return True if cuda will be used otherwise false.
             */
            virtual bool isCuda() = 0;

        protected:

            /**
             * Indicator how much matches need to get an good matching result. Default value is 40,
             */
            int countMatches = 40;

            /**
             * Indicator to use IRA algorithm.
             */
            bool useIRA = false;
        };
    }
}




#endif //COMPANION_ABSTRACTFEATUREMATCHING_H
