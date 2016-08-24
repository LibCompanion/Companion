#include "flann.h"

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Flann::Flann()
{
	this->accordance = 1;
}

Flann::Flann(string search_image_path, string compare_image_path, double accordance, vector<DMatch> matches, vector<KeyPoint> keypoints_1, vector<KeyPoint> keypoints_2)
{
	this->search_image_path = search_image_path;
	this->compare_image_path = compare_image_path;
	this->accordance = accordance;
	this->matches = matches;
	this->keypoints_1 = keypoints_1;
	this->keypoints_2 = keypoints_2;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Flann::~Flann()
{
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Public Methods                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void Flann::set_matches(vector<DMatch> matches)
{
	this->matches = matches;
}

vector<DMatch> Flann::get_matches()
{
	return this->matches;
}

void Flann::show_compare_points()
{
	Mat img_1;
	Mat img_2;
	Mat img_matches;

	if (search_image_path.empty() || compare_image_path.empty())
	{
		return;
	}

	img_1 = imread(search_image_path);
	img_2 = imread(compare_image_path);

	drawMatches(img_1, keypoints_1, img_2, keypoints_2,
		matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Show detected matches
	imshow("Good Matches", img_matches);
	waitKey(0);

	img_1.release();
	img_2.release();
	img_matches.release();
}
