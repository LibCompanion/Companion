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

/**
 * Comparison data model to store search results from an image recognition.
 * @author Andreas Sekulski
 */
class Comparison {

public:

    // ToDo Model name and abstract class implementation.

    Comparison();

    ~Comparison();

    const cv::Mat &getImage() const;

    void setImage(const cv::Mat &image);

    const cv::Rect &getLastPosition() const;

    void setLastPosition(int x, int y, int width, int height);

    void setLastPositionWidth(int width);

    void setLastPositionHeight(int height);

    void setLastPositionX(int x);

    void setLastPositionY(int y);

    bool isLastPositionSet();

    const cv::Mat &getDescriptors() const;

    void setDescriptors(const cv::Mat &descriptors);

    const std::vector<cv::KeyPoint> &getKeypoints() const;

    void setKeypoints(const std::vector<cv::KeyPoint> &keypoints);

private:

    cv::Mat image;

    cv::Rect lastPosition;

    cv::Mat descriptors;

    std::vector<cv::KeyPoint> keypoints;

};

#endif //COMPANION_COMPARISON_H