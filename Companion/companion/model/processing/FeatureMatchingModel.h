/*
 * This program is an object recognition framework written with OpenCV.
 * Copyright (C) 2016-2018 Andreas Sekulski, Dimitri Kotlovsky
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

#ifndef COMPANION_FEATUREMATCHINGMODEL_H
#define COMPANION_FEATUREMATCHINGMODEL_H

#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <companion/algo/recognition/matching/util/IRA.h>

namespace Companion { namespace Model { namespace Processing
{
    /**
     * Comparison data model to store search results from a feature matching algorithm.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS FeatureMatchingModel
    {

    public:

        /**
         * Constructor.
         */
        FeatureMatchingModel();

        /**
         * Destructor.
         */
        virtual ~FeatureMatchingModel();

        /**
         * Gets the descriptors from feature matching if exists.
         * @return An empty cv::Mat if no matching exists, otherwise a cv::Mat of descriptors.
         */
        const cv::Mat &getDescriptors() const;

        /**
         * Sets feature matching descriptors.
         * @param descriptors Feature matching descriptors to set.
         */
        void setDescriptors(const cv::Mat &descriptors);

        /**
         * Gets all keypoints from matching.
         * @return If keypoints do not exist, keypoints is empty otherwise a filled keypoints vector.
         */
        const std::vector<cv::KeyPoint> &getKeypoints() const;

        /**
         * Checks if keypoints are already calculated.
         * @return <code>True</code> if keypoints are calculated otherwise <code>false</code>
         */
        bool keypointsCalculated();

        /**
         * Sets given keypoints from matching.
         * @param keypoints Keypoints to set.
         */
        void setKeypoints(const std::vector<cv::KeyPoint> &keypoints);

        /**
         * Calculates cv::Mat keypoints and descriptors from given detector and extractor and stores them. This operation
         * does not work for Cuda feature detectors, because they need cv::gpu::Mat.
         * @param detector Detector to use.
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
         * Gets IRA class to store last recognized object's location.
         * @return IRA class to obtain informations about last recognized object' location.
         */
        Companion::Algorithm::Recognition::Matching::UTIL::IRA *getIra() const;

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
         * Image reduction algorithm to store last recognized object's location.
         */
        Companion::Algorithm::Recognition::Matching::UTIL::IRA *ira;

    };
}}}

#endif //COMPANION_FEATUREMATCHINGMODEL_H
