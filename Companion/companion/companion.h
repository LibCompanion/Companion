#pragma once

#include "search\compare.h"
#include "search\flann.h"
#include "search\templatematch.h"

#include <omp.h>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\nonfree\features2d.hpp>

using namespace cv;
using namespace std;

/// Enumeration class for errors
enum Error
{
	image_not_found, ///< If image not found error
	dimension_error, ///< If dimensions from given images inequal
	template_dimension_error ///< If dimensions from template is wrong
};

/**
 * @brief Companion class to use image recognition methods like simple compare, template matching or flann.
 * @author Andreas Sekulski
 */
class Companion
{
public:

	/**
	 * @brief Companion default constructor.
	 */
	Companion();

	/**
	 * @brief Destroys the object.
	 */
	~Companion();

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
	Compare search_compare_image_mp(string search_img_path, vector<string> compare_image_paths, double min_threshold);

	/**
	 * @brief Simple search compares compare_img_path for similarity.<br>
	 *        Best matches are high results and approach goes to one.
	 *
	 * @param search_img_path   The search image path to check for similarity.
	 * @param compare_img_path  The compare image path to check for similarity.
	 * @throws Error If an error occured in search operation.
	 *
	 * @return Gives an compare result model with an given accordance to search_img_path and compare_img_path.
	 */
	Compare search_compare_image(string search_img_path, string compare_img_path);

	/**
	 * @brief Multi-Processing template matching search.<br> 
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
	TemplateMatch search_template_matching_mp(string search_img_path, vector<string> compare_img_paths, double threshold, int match_method, bool resize_same_size);

	/**
	 * @brief Template matching search implementation from <a href="http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/template_matching/template_matching.html">OpenCV</a>.<br>
	 *        Results threshold depends on match_method.
	 *
	 * @param search_img_path   The search image path to check for similarity.
	 * @param template_img_path The compare image path to check for similarity.
	 * @param match_method      The match method which OpenCV method search operation should be used.<br>
	 *                          Following operations can be used CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED.<br>
	 *                          For SQDIFF and SQDIFF_NORMED, the best matches are lower values.<br>
	 *                          For all the other methods, the higher the better
	 * @param resize_same_size  If true images will be resized to same size otherwise images will not be resized.
	 *
	 * @throws Error If an error occured in search operation.
	 *
	 * @return Gives an template match result model with an given accordance to search_img_path and compare_img_path.
	 */
	TemplateMatch search_template_matching(string search_img_path, string template_img_path, int match_method, bool resize_same_size);

	/**
	 * @brief Multi-Processing fast library for approximate nearest neighbors search.<br>
	 *        Best matches are low results and approach goes to zero.
	 *
	 * @param search_img_path   The search image path to check for similarity.
	 * @param compare_img_paths The compare_img_paths contails all paths to images to compare with search image.
	 * @param min_threshold     The minimum threshold indicator if an result is good enough.
	 *
	 * @return Gives best flann match object if exists otherwise an empty compare object.
	 */
	Flann search_flann_mp(string search_img_path, vector<string> compare_img_paths, double min_threshold);

	/**
	 * @brief Fast library for approximate nearest neighbors search from <a href="http://docs.opencv.org/2.4/doc/tutorials/features2d/feature_flann_matcher/feature_flann_matcher.html">OpenCV</a>.<br>
	 *        Best matches are low results and approach goes to zero.
	 * 
	 * @param search_img_path  The search image path to check for similarity.
	 * @param compare_img_path The compare image path to check for similarity.
	 * @throws Error If an error occured in search operation.
	 *
	 * @return Gives an flann match result model with an given accordance to search_img_path and compare_img_path.
	 */
	Flann search_flann(string search_img_path, string compare_img_path);

	/**
	 * @brief Gets from given error an corresponding error message.
	 *
	 * @param error_code The error code to get corresponding error message.
	 *
	 * @return String message from given error.
	 */
	string get_error(Error error_code);

private:

	/**
	 * @brief Resize both given image to same size.
	 *
	 * @param img1 Image to resize.
	 * @param img2 Image to resize.
	 */
	void resize_image_equal(Mat &img1, Mat &img2);

	/**
	 * @brief Sets given image to size.
	 *
	 * @param img     The image to resize.
	 * @param size_x  The size x to resize.
	 * @param size_y  The size y to resize.
	 */
	void resize_image(Mat &img, int size_x, int size_y);

};
