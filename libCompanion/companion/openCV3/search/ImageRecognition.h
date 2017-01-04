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

#ifndef COMPANION_IMAGERECOGNITION_H
#define COMPANION_IMAGERECOGNITION_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <companion/openCV3/util/CompanionError.h>
#include <companion/openCV3/model/Comparison.h>
#include <companion/openCV3/util/Util.h>

/**
 * Image recognition abstract class to implement specific image recognition algorithms.
 * @author Andreas Sekulski
 */
class ImageRecognition {

public:

    /**
     * Image recognition method to obtain comparison from images.
     * @param search_image_path Path to image file.
     * @param compare_image_path Path to compare image file.
     * @param cv_color OpenCV color type like CV_LOAD_IMAGE_GRAYSCALE, CV_LOAD_IMAGE_COLOR, CV_LOAD_IMAGE_ANYDEPTH or CV_LOAD_IMAGE_ANYCOLOR.
     * @throws CompanionError::error_code If an error occured in search operation.
     * @return An comparison model which contains all search results from algo.
     */
    Comparison *search(std::string search_image_path, std::string compare_image_path, int cv_color);

    /**
     * Image recognition method to obtain comparison from two loaded images.
     * @param search_img Image file to compare.
     * @param compare_img Compare image file to check search image file.
     * @throws CompanionError::error_code If an error occured in search operation.
     * @return An comparison model which contains all search results from algo.
     */
    Comparison *search(cv::Mat search_img, cv::Mat compare_img);

protected:

    /**
     * Specific algorithm implementation like simple compare or template matching.
     * @param search_img Image file to compare.
     * @param compare_img Compare image file to check search image file.
     * @throws CompanionError::error_code If an error occured in search operation.
     * @return An comparison model which contains all search results from algo.
     */
    virtual Comparison *algo(cv::Mat search_img, cv::Mat compare_img) = 0;
};

#endif //COMPANION_IMAGERECOGNITION_H