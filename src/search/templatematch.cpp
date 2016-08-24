#include "templatematch.h"

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TemplateMatch::TemplateMatch()
{
	this->accordance = 0;
}

TemplateMatch::TemplateMatch(string search_image_path, string compare_image_path, double accordance, Point match_point, Point match_offset)
{
	this->search_image_path = search_image_path;
	this->compare_image_path = compare_image_path;
	this->accordance = accordance;
	this->match_point = match_point;
	this->match_offset = match_offset;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TemplateMatch::~TemplateMatch()
{
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Public Methods                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void TemplateMatch::show_compare_points()
{
	Mat img_display;
	Mat search_img;

	if (search_image_path.empty())
	{
		return;
	}

	search_img = imread(search_image_path);
	search_img.copyTo(img_display);

	char* image_window = "Template Match";

	/// Show me what you got
	rectangle(img_display, match_point, match_offset, Scalar::all(0), 2, 8, 0);

	/// Create windows
	namedWindow(image_window, CV_WINDOW_AUTOSIZE);

	imshow(image_window, img_display);

	waitKey(0);

	search_img.release();
}
