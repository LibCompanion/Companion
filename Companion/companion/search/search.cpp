#include "search.h"

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Public Methods                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void Search::show_images()
{
	Mat searched_img = imread(this->search_image_path);
	Mat find_img = imread(this->compare_image_path);

	Mat img_display;
	Mat tmp_display;

	char* image_window = "Searched Image";
	char* template_window = "Found Image";

	namedWindow(image_window, CV_WINDOW_AUTOSIZE);
	namedWindow(template_window, CV_WINDOW_AUTOSIZE);

	searched_img.copyTo(img_display);
	find_img.copyTo(tmp_display);

	imshow(image_window, img_display);
	imshow(template_window, tmp_display);
	waitKey(0);

	searched_img.release();
	find_img.release();
}

void Search::set_search_image_path(string search_image_path)
{
	this->search_image_path = search_image_path;
}

string Search::get_search_image_path()
{
	return this->search_image_path;
}

void Search::set_compare_image_path(string compare_image_path)
{
	this->compare_image_path = compare_image_path;
}

string Search::get_compare_image_path()
{
	return this->compare_image_path;
}

void Search::set_accordance(double accordance)
{
	this->accordance = accordance;
}

double Search::get_accordance()
{
	return this->accordance;
}