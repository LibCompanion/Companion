#include "companion.h"

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Companion::Companion()
{
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Companion::~Companion()
{
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Public Methods                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Compare Companion::search_compare_image_mp(string search_img_path, vector<string> compare_image_paths, double min_threshold)
{
	Compare best_image;

	#pragma omp parallel 
	{
		// ToDo := Stop if card found with 100%
		for (auto const& check_image_path : compare_image_paths)
		{
			#pragma omp single nowait 
			{
				try 
				{
					Compare result = search_compare_image(search_img_path, check_image_path);
					if (result.get_accordance() >= min_threshold && result.get_accordance() > best_image.get_accordance())
					{
						#pragma omp critical
						{
							best_image = result;
						}
					}
				}
				catch (Error e)
				{
					// ToDo := Better Error Handling
					cout << get_error(e) << "\n";
				}
			}
		}
	}

	return best_image;
}

Compare Companion::search_compare_image(string search_img_path, string compare_img_path)
{
	int pixels;
	int equalPixels;
	double accordance;

	Mat result;
	Mat img = imread(search_img_path, CV_LOAD_IMAGE_GRAYSCALE);
	Mat compare_img = imread(compare_img_path, CV_LOAD_IMAGE_GRAYSCALE);

	// Check if image is loaded
	if (img.empty() || compare_img.empty())
	{
		throw Error::image_not_found;
	}

	// Resize image to same size
	resize_image_equal(img, compare_img);

	// Check if dimension from img is equal to compare_image
	if (img.cols != compare_img.cols || img.rows != compare_img.rows || img.dims != compare_img.dims)
	{
		throw Error::dimension_error;
	}

	// Calculate pixel size from image.
	pixels = img.cols * img.rows;

	// Compare images for equality
	compare(img, compare_img, result, CMP_EQ);

	equalPixels = countNonZero(result);
	accordance = (double) equalPixels / (double) pixels;

	img.release();
	compare_img.release();

	return Compare::Compare(search_img_path, compare_img_path, accordance);
}

TemplateMatch Companion::search_template_matching_mp(string search_img_path, vector<string> compare_img_paths, double threshold, int match_method, bool resize_same_size)
{
	TemplateMatch best_image = TemplateMatch::TemplateMatch();

	#pragma omp parallel 
	{
		// ToDo := Stop if card found with 100%
		for (auto const& check_image_path : compare_img_paths)
		{
			#pragma omp single nowait 
			{
				try
				{
					TemplateMatch result = search_template_matching(search_img_path, check_image_path, match_method, resize_same_size);
					// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
					if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) 
					{

						if (result.get_accordance() <= threshold 
							&& (result.get_accordance() < best_image.get_accordance() || best_image.get_compare_image_path().empty()))
						{
							#pragma omp critical
							{
								best_image = result;
							}
						}
					}
					else {
						if (result.get_accordance() >= threshold 
							&& (result.get_accordance() > best_image.get_accordance() || best_image.get_compare_image_path().empty()))
						{
							#pragma omp critical
							{
								best_image = result;
							}
						}
					}
				}
				catch (Error e)
				{
					// ToDo := Better Error Handling
					cout << get_error(e) << "\n";
				}
			}
		}
	}

	return best_image;
}

TemplateMatch Companion::search_template_matching(string search_img_path, string template_img_path, int match_method, bool resize_same_size)
{
	Mat search_img = imread(search_img_path);
	Mat template_img = imread(template_img_path);
	Mat result;

	// Check if image is loaded
	if (search_img.empty() || template_img.empty())
	{
		throw Error::image_not_found;
	}

	if (resize_same_size) 
	{
		resize_image_equal(search_img, template_img);
	}	

	// If template is greater than image size
	if (search_img.cols < template_img.cols || search_img.rows < template_img.rows)
	{
		throw Error::template_dimension_error;
	}

	// Create the result matrix
	int result_cols = search_img.cols - template_img.cols + 1;
	int result_rows = search_img.rows - template_img.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	// Methods : CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED
	matchTemplate(search_img, template_img, result, match_method);
	threshold(result, result, 0.1, 1., CV_THRESH_TOZERO);

	// Localizing the best match with minMaxLoc
	double minVal;
	double maxVal;

	Point minLoc;
	Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	return TemplateMatch::TemplateMatch(search_img_path, template_img_path, maxVal);
}

Flann Companion::search_flann_mp(string search_img_path, vector<string> compare_img_paths, double min_threshold)
{
	Flann best_image = Flann::Flann();

	#pragma omp parallel 
	{
		// ToDo := Stop if card found with 100%
		for (auto const& check_image_path : compare_img_paths)
		{
			#pragma omp single nowait 
			{
				try
				{
					Flann result = search_flann(search_img_path, check_image_path);
					if (result.get_accordance() < min_threshold
						&& result.get_accordance() < best_image.get_accordance()
						&& result.get_matches_size() > best_image.get_matches_size())
					{
						#pragma omp critical
						{
							best_image = result;
						}
					}
				}
				catch (Error e)
				{
					// ToDo := Better Error Handling
					cout << get_error(e) << "\n";
				}
				
			}
		}
	}

	return best_image;
}

Flann Companion::search_flann(string search_img_path, string compare_img_path)
{
	Mat img_1 = imread(search_img_path);
	Mat img_2 = imread(compare_img_path);

	// Check if image is loaded
	if (img_1.empty() || img_2.empty()) {
		throw Error::image_not_found;
	}

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	SurfFeatureDetector detector(minHessian);

	vector<KeyPoint> keypoints_1, keypoints_2;

	detector.detect(img_1, keypoints_1);
	detector.detect(img_2, keypoints_2);

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_1, descriptors_2;

	extractor.compute(img_1, keypoints_1, descriptors_1);
	extractor.compute(img_2, keypoints_2, descriptors_2);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	double max_dist = 0;
	double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_1.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist)
		{
			min_dist = dist;
		}
		if (dist > max_dist)
		{
			max_dist = dist;
		}
	}

	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	//-- small)
	//-- PS.- radiusMatch can also be used here.
	vector<DMatch> good_matches;
	double good_matches_sum = 0.0;

	for (int i = 0; i < descriptors_1.rows; i++) {
		if (matches[i].distance <= max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
			good_matches_sum += matches[i].distance;
		}
	}

	// If score = 0 img is equal > 0 threshold decision
	double score = good_matches_sum / good_matches.size();

	img_1.release();
	img_2.release();

	return Flann::Flann(search_img_path, compare_img_path, score, good_matches.size());
}

string Companion::get_error(Error error_code)
{
	string error = "";
	switch (error_code)
	{
	case image_not_found : 
		error = "Could not open or find image";
		break;
	case dimension_error:
		error = "Dimensions not equal";
		break;
	case template_dimension_error:
		error = "Template size must be smaller or equal than image";
		break;
	}

	return error;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Private Methods                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void Companion::resize_image_equal(Mat &img1, Mat &img2)
{
	Mat resized;
	int x;
	int y;

	// Check if size from img is equal to compare image
	if (img1.cols == img2.cols && img1.rows == img2.rows) 
	{
		return;
	}

	if (img1.cols > img2.cols) 
	{
		x = img1.cols;
	}
	else {
		x = img2.cols;
	}

	if (img1.rows > img2.rows) 
	{
		y = img1.rows;
	}
	else 
	{
		y = img2.rows;
	}

	resize(img1, img1, Size(x, y));
	resize(img2, img2, Size(x, y));
}

void Companion::resize_image(Mat &img, int size_x, int size_y)
{
	resize(img, img, Size(size_x, size_y));
}
