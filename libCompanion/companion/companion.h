#pragma once

#include <omp.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include "companion/openCV3/util/CompanionError.h"

/**
 * @brief Companion class to use image recognition methods like simple compare, template matching or flann.
 * @author Andreas Sekulski
 */
class Companion
{
public:

	/* ToDo Documentation */
	void calc_histogram(std::string img_path);

	/**
	 * @brief Multi-Processing simple search compare.<br>
	 *        Best matches are high results and approach goes to one.
	 *
	 * @param search_img_path     The search image path to check for similarity.
	 * @param compare_image_paths The compare_img_paths contails all paths to images to compare with search image.
	 * @param min_threshold       Minimum threshold result which should accept image compare for an comparison.
	 *
	 * @return Gives best compare object if exists otherwise an empty compare object.
	 */
    void search_compare_image_mp(std::string search_img_path, std::vector<std::string> compare_image_paths, double min_threshold);

	/**
	 * @brief Template matching search which used openmp multi processing or cuda. Use of cuda will be faster than multi processing.<br>
	 *        Best matches depends on match_method.
	 *
	 * @param search_img_path   The search image path to check for similarity.
	 * @param compare_img_paths The compare_img_paths contails all paths to images to compare with search image.
	 * @param threshold         The threshold indicator if an result is good enough.
	 * @param match_method      The match method which OpenCV method search operation should be used.<br>
	 *                          Following operations can be used CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED.<br>
	 *                          For SQDIFF and SQDIFF_NORMED, the best matches are lower values.<br>
	 *                          For all the other methods, the higher the better
	 * @param resize_same_size  If true images will be resized to same size otherwise images will not be resized.
	 *
	 * @return Gives best template match object if exists otherwise an empty compare object.
	 */
    void search_vector_template_matching(std::string search_img_path, std::vector<std::string> compare_img_paths, double threshold, int match_method, bool resize_same_size);

	/**
	 * @brief Multi-Processing feature matching search.<br>
	 *        Best matches are low results and approach goes to zero.
	 *
	 * @param search_img_path   The search image path to check for similarity.
	 * @param compare_img_paths The compare_img_paths contails all paths to images to compare with search image.
	 * @param min_threshold     The minimum threshold indicator if an result is good enough.
	 *
	 * @return Gives best flann match object if exists otherwise an empty compare object.
	 */
    void search_feature_matching_mp(std::string search_img_path, std::vector<std::string> compare_img_paths, double min_threshold);

private:

	/**
	 * @brief Resize both given image to same size.
	 *
	 * @param img1 Image to resize.
	 * @param img2 Image to resize.
	 */
	void resize_image_equal(cv::Mat &img1, cv::Mat &img2);

	/**
	 * @brief Sets given image to size.
	 *
	 * @param img     The image to resize.
	 * @param size_x  The size x to resize.
	 * @param size_y  The size y to resize.
	 */
	void resize_image(cv::Mat &img, int size_x, int size_y);
};
