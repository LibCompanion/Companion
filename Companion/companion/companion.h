#pragma once

#include "search\compare.h"
#include "search\flann.h"
#include "search\templatematch.h"

#include <omp.h>
#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\opencv_modules.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>

using namespace cv;
using namespace std;

enum Error
{
	image_not_found,
	dimension_error,
	template_dimension_error
};

class Companion
{
public:
	
	Companion();

	~Companion();

	Compare search_compare_image_mp(string img_path, vector<string> images, double min_threshold);

	Compare search_compare_image(string search_img_path, string compare_img_path);

	TemplateMatch search_template_matching_mp(string img_path, vector<string> images, double threshold, int match_method);

	TemplateMatch search_template_matching(string search_img_path, string template_img_path, int match_method);

	Flann search_flann_mp(string img_path, vector<string> images, double min_threshold);

	Flann search_flann(string search_img_path, string compare_img_path);

	void show_cards(string img1, string img2);

	string get_error(Error error_code);

private:
	void resize_image_equal(Mat &img1, Mat &img2);

	void resize_image(Mat &img, int size_x, int size_y);
};
