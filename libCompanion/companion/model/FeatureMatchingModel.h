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

#include "ImageRecognitionModel.h"

/**
 * Comparison data model to store search results from an feature matching algo.
 * @author Andreas Sekulski
 */
class FeatureMatchingModel : public ImageRecognitionModel {

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

private:

    /**
     * Feature descriptors from matching.
     */
    cv::Mat descriptors;

    /**
     * Keypoints from matching.
     */
    std::vector<cv::KeyPoint> keypoints;

};

#endif //COMPANION_COMPARISON_H