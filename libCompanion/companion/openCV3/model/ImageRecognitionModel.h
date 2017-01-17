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

#ifndef COMPANION_IMAGERECOGNITIONMODEL_H
#define COMPANION_IMAGERECOGNITIONMODEL_H

#include <opencv2/core/core.hpp>

/**
 * Default model type for an image recognition implementation class for an scene or object.
 * @author Andreas Sekulski
 */
class ImageRecognitionModel {

public:

    /**
     * Creates an default image recognition model class.
     */
    ImageRecognitionModel();

    /**
     * Create an default image recognition model with an given image.
     * @param image Image to set.
     */
    ImageRecognitionModel(cv::Mat image);

    /**
     * Destructor.
     */
    virtual ~ImageRecognitionModel();

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
     * Gets last object detection position if exists.
     * @return Gets last object detection position if exists, otherwise position is zero.
     */
    const cv::Rect &getLastPosition() const;

    /**
     * Sets last position from object detection.
     * @param x X-Axis position from object.
     * @param y Y-Axis position from object.
     * @param width Width from object.
     * @param height Height from object.
     */
    void setLastPosition(int x, int y, int width, int height);

    /**
     * Sets width from detected object.
     * @param width Width from homography object.
     */
    void setLastPositionWidth(int width);

    /**
     * Sets height from detected object.
     * @param height Height from homography object.
     */
    void setLastPositionHeight(int height);

    /**
     * Sets x position from object.
     * @param x X position from homography object.
     */
    void setLastPositionX(int x);

    /**
     * Sets y position from object.
     * @param y Y position from homography object.
     */
    void setLastPositionY(int y);

    /**
     * Checks if last position from object exists.
     * @return <b>true<br> if last position is set otherwise <b>false</b>
     */
    bool isLastPositionSet();

protected:

    /**
     * Image to store.
     */
    cv::Mat image;

    /**
     * Last position from detected object.
     */
    cv::Rect lastPosition;
};

#endif //COMPANION_IMAGERECOGNITIONMODEL_H