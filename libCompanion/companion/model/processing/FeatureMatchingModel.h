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

#ifndef COMPANION_COMPARISON_H
#define COMPANION_COMPARISON_H

#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <companion/algo/matching/util/IRA.h>

namespace Companion { namespace Model { namespace Processing
{
    /**
     * Comparison data model to store search results from an feature matching algo.
     * @author Andreas Sekulski
     */
    class COMP_EXPORTS FeatureMatchingModel
    {

    public:

        /**
         * Constructor to create an feature matching model.
         */
        FeatureMatchingModel();

        /**
         * Destructor from feature matching model.
         */
        virtual ~FeatureMatchingModel();

        /**
         * Get descriptors from feature matching if exists.
         * @return An empty cv::Mat descriptor if no matching exists otherwise an cv::Mat descriptor.
         */
        const cv::Mat &getDescriptors() const;

        /**
         * Sets an descriptor matching.
         * @param descriptors Descriptor matching to set.
         */
        void setDescriptors(const cv::Mat &descriptors);

        /**
         * Get all keypoints from matching.
         * @return If keypoints not exists keypoints is empty otherwise an filled keypoints set.
         */
        const std::vector<cv::KeyPoint> &getKeypoints() const;

        /**
         * Checks if keypoints are already calculated.
         * @return <b>True</b> if keypoints are calculated otherwise <b>False</b>
         */
        bool keypointsCalculated();

        /**
         * Sets given keypoints from matching.
         * @param keypoints Keypoints to set.
         */
        void setKeypoints(const std::vector<cv::KeyPoint> &keypoints);

        /**
         * Calculates cv::Mat keypoints and descriptors from given detector and extractor and stores this. This operation
         * not working for Cuda feature detectors because they need cv::gpu::Mat.
         * @param detector FeatureDetector to use.
         * @param extractor Extractor to use.
         */
        void calculateKeyPointsAndDescriptors(cv::Ptr<cv::FeatureDetector> detector,
            cv::Ptr<cv::DescriptorExtractor> extractor);

        /**
         * Gets image which is stored, if no image is stored image is empty.
         * @return An image if is set otherwise image is empty.
         */
        const cv::Mat &getImage() const;

        /**
         * Sets given image.
         * @param image Image to set.
         */
        void setImage(const cv::Mat &image);

        /**
         * Gets IRA class to store last object detection.
         * @return IRA class to obtain informations about last object detection.
         */
        Companion::Algorithm::Matching::UTIL::IRA *getIra() const;

        /**
         * Sets the ID for this model.
         * @param id ID to set.
         */
        void setID(int id);

        /**
         * Gets the ID of this model.
         * @return  The ID of this model.
         */
        const int getID() const;

    private:

        /**
         * Feature descriptors from matching.
         */
        cv::Mat descriptors;

        /**
         * Keypoints from matching.
         */
        std::vector<cv::KeyPoint> keypoints;

        /**
         * The ID of this model.
         */
        int id;

        /**
         * Image to store.
         */
        cv::Mat image;

        /**
         * Image reduction algorithm to store last object detection position.
         */
        Companion::Algorithm::Matching::UTIL::IRA *ira;

    };
}}}

#endif //COMPANION_COMPARISON_H