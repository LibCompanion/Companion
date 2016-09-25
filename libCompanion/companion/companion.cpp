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

void Companion::calc_histogram(string img_path)
{
	Mat src, dst;

	/// Load image
	src = imread(img_path, 1);

	if (src.empty())
	{
		return;
	}

	/// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	/// Display
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);

	waitKey(0);
}


Compare *Companion::search_compare_image_mp(string search_img_path, vector<string> compare_image_paths, double min_threshold)
{
    Compare *best_image = new Compare();

    #pragma omp parallel
	{
		// ToDo := Stop if card found with 100%
		for (auto const& check_image_path : compare_image_paths)
		{
            #pragma omp single nowait
			{
				try 
				{
                    Compare *result = search_compare_image(search_img_path, check_image_path);

                    if (result->get_accordance() >= min_threshold && result->get_accordance() > best_image->get_accordance())
                    {
                        #pragma omp critical
                        {
							delete best_image;
                            best_image = result;
                        }
                    }
					else
					{
						delete result;
					}
				}
                catch (error_code e)
				{
					// ToDo := Better Error Handling
					cout << get_error(e) << "\n";
				}
			}
		}
	}

	return best_image;
}

Compare *Companion::search_compare_image(string search_img_path, string compare_img_path)
{
	int pixels;
	int equalPixels;
	double accordance;

	Mat result;
	Mat img = imread(search_img_path, CV_LOAD_IMAGE_GRAYSCALE);
	Mat compare_img = imread(compare_img_path, CV_LOAD_IMAGE_GRAYSCALE);

	// Check if image is loaded
	if (!is_image_loaded(img) || !is_image_loaded(compare_img))
	{
        throw error_code::image_not_found;
	}

	// Resize image to same size
	resize_image_equal(img, compare_img);

	// Check if dimension from img is equal to compare_image
	if (img.cols != compare_img.cols || img.rows != compare_img.rows || img.dims != compare_img.dims)
	{
        throw error_code::dimension_error;
	}

	// Calculate pixel size from image.
	pixels = img.cols * img.rows;

	// Compare images for equality
	compare(img, compare_img, result, CMP_EQ);

	equalPixels = countNonZero(result);
	accordance = (double) equalPixels / (double) pixels;

	img.release();
	compare_img.release();

    return new Compare(search_img_path, compare_img_path, accordance);
}

TemplateMatch *Companion::search_vector_template_matching(string search_img_path, vector<string> compare_img_paths, double threshold, int match_method, bool resize_same_size)
{
	TemplateMatch *best_image = new TemplateMatch();

	#pragma omp parallel 
	{
		// ToDo := Stop if card found with 100%
		for (auto const& check_image_path : compare_img_paths)
		{
			#pragma omp single nowait 
			{
				try
				{
					TemplateMatch *result = search_template_matching(search_img_path, check_image_path, match_method, resize_same_size);
					// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
					if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
					{

						if (result->get_accordance() <= threshold
							&& (result->get_accordance() < best_image->get_accordance() || best_image->get_compare_image_path().empty()))
						{
							#pragma omp critical
							{
								best_image = result;
							}
						}
					}
					else {
						if (result->get_accordance() >= threshold
							&& (result->get_accordance() > best_image->get_accordance() || best_image->get_compare_image_path().empty()))
						{
							#pragma omp critical
							{
								delete best_image;
								best_image = result;
							}
						}
						else
						{
							delete result;
						}
					}
				}
                catch (error_code e)
				{
					// ToDo := Better Error Handling
					cout << get_error(e) << "\n";
				}
			}
		}
	}

	return best_image;
}

TemplateMatch *Companion::search_template_matching(string search_img_path, string template_img_path, int match_method, bool resize_same_size)
{
	Mat search_img = imread(search_img_path);
	Mat template_img = imread(template_img_path);
	Mat result;

	// Check if image is loaded
	if (!is_image_loaded(search_img) || !is_image_loaded(template_img))
	{
        throw error_code::image_not_found;
	}

	if (resize_same_size) 
	{
		resize_image_equal(search_img, template_img);
	}	

	// If template is greater than image size
	if (search_img.cols < template_img.cols || search_img.rows < template_img.rows)
	{
        throw error_code::template_dimension_error;
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
	double accordance;

	Point minLoc;
	Point maxLoc;
	Point matchLoc;
	Point matchOffset;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		accordance = minVal;
	}
	else
	{
		matchLoc = maxLoc;
		accordance = maxVal;
	}

	matchOffset = Point(matchLoc.x + template_img.cols, matchLoc.y + template_img.rows);

	search_img.release();
	template_img.release();

	return new TemplateMatch(search_img_path, template_img_path, accordance, matchLoc, matchOffset);
}

FeatureMatch* Companion::search_feature_matching_mp(string search_img_path, vector<string> compare_img_paths, double min_threshold, detector detector, extractor extractor, matcher matcher)
{
	FeatureMatch *best_image = new FeatureMatch();

	#pragma omp parallel 
	{
		// ToDo := Stop if card found with 100%
		for (auto const& check_image_path : compare_img_paths)
		{
			#pragma omp single nowait 
			{
				try
				{
					FeatureMatch *result = search_feature_matching(search_img_path, check_image_path, detector, extractor, matcher);
					double accordance = result->get_accordance();
					size_t matches_size = result->get_matches().size();
					double compare_accordance = best_image->get_accordance();
					size_t compare_matches_size = best_image->get_matches().size();

					if (accordance < min_threshold && accordance < compare_accordance && matches_size > compare_matches_size)
					{
						#pragma omp critical
						{
							// Delete last result and store new best result
							delete best_image;
							best_image = result;
						}
					}
					else
					{
						// Delete result
						delete result;
					}
				}
                catch (error_code e)
				{
					// ToDo := Better Error Handling
					cout << get_error(e) << "\n";
				}
				
			}
		}
	}

	return best_image;
}

FeatureMatch* Companion::search_feature_matching(string search_img_path, string compare_img_path, detector detector, extractor extractor, matcher matcher)
{
	Mat img_object;
	Mat img_scene;
	string fd_type; // Feature detector type
	string de_type; // Descriptor extractor type
	string dm_type; // Decriptor matcher type

	try
	{
		// Check if given solution set from feature matching is supported.
		fd_type = get_feature_detector(detector);
		de_type = get_descriptor_extractor(extractor);
		dm_type = get_decriptor_matcher(matcher);
	}
    catch (error_code e)
	{
		throw e;
	}

	img_object = imread(search_img_path, CV_LOAD_IMAGE_GRAYSCALE);
	img_scene = imread(compare_img_path, CV_LOAD_IMAGE_GRAYSCALE);
	// Check if images are loaded
	if (!is_image_loaded(img_object) || !is_image_loaded(img_scene)) {
        throw error_code::image_not_found;
	}

	// ToDo : For FeatureDetector, DescriptorExtractor and DescriptorMatcher check to setup variable indicators.

	//-- Step 1: Detect the keypoints using SURF Detector
	Ptr<FeatureDetector> feature_detector = FeatureDetector::create(fd_type);
	vector<KeyPoint> keypoints_object, keypoints_scene;
	feature_detector->detect(img_object, keypoints_object);
	feature_detector->detect(img_scene, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)
	Ptr<DescriptorExtractor> descriptor_extractor = DescriptorExtractor::create(de_type);
	Mat descriptors_object, descriptors_scene;
	descriptor_extractor->compute(img_object, keypoints_object, descriptors_object);
	descriptor_extractor->compute(img_scene, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors
	Ptr<DescriptorMatcher> descriptor_matcher = DescriptorMatcher::create(dm_type);
	vector<DMatch> matches;
	descriptor_matcher->match(descriptors_object, descriptors_scene, matches);

	//-- Step 4: Result creating for matching
	double max_dist = 0;
	double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++)
	{
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
	double score = 1;

	for (int i = 0; i < descriptors_object.rows; i++) {
		if (matches[i].distance <= max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
			good_matches_sum += matches[i].distance;
		}
	}

	// If score = 0 img is equal > 0 threshold decision
	if (good_matches.size() > 0)
	{
		score = good_matches_sum / good_matches.size();
		if (score > 1)
		{
			score = 1;
		}
		else if (score < 0)
		{
			score = 0;
		}
	}

	img_object.release();
	img_scene.release();
	
	return new FeatureMatch(search_img_path, compare_img_path, score, good_matches, keypoints_object, keypoints_scene);
}

string Companion::get_error(error_code e_code)
{
	string error = "";
    switch (e_code)
    {
    case error_code::image_not_found:
		error = "Could not open or find image";
		break;
    case error_code::dimension_error:
		error = "Dimensions not equal";
		break;
    case error_code::template_dimension_error:
		error = "Template size must be smaller or equal than image";
		break;
    case error_code::descriptor_extractor_not_found:
		error = "Given descriptor not supported";
		break;
    case error_code::feature_detector_not_found:
		error = "Given feature not supported";
		break;
	default:
		error = "Unknown error";
		break;
	}

	return error;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Private Methods                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void Companion::resize_image_equal(Mat &img1, Mat &img2)
{
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

bool Companion::is_image_loaded(Mat &img) 
{
	return !img.empty();
}

string Companion::get_feature_detector(detector f_detector)
{
	string feature;

    switch (f_detector)
	{
        case detector::FAST:
			feature = "FAST";
			break;
        case detector::STAR:
			feature = "STAR";
			break;
        case detector::ORB:
			feature = "ORB";
			break;
        case detector::BRISK:
			feature = "BRISK";
			break;
        case detector::MSER:
			feature = "MSER";
			break;
        case detector::GFTT:
			feature = "GFTT";
			break;
        case detector::HARRIS:
			feature = "HARRIS";
			break;
        case detector::Dense:
			feature = "Dense";
			break;
        case detector::SimpleBlob:
			feature = "SimpleBlob";
			break;
		default:
            throw error_code::feature_detector_not_found;
			break;
	}

	return feature;
}

string Companion::get_descriptor_extractor(extractor f_extractor)
{
	string extractor_type;

    switch (f_extractor)
	{
    case extractor::SIFT:
		extractor_type = "SIFT";
		break;
    case extractor::SURF:
		extractor_type = "SURF";
		break;
    case extractor::BRIEF:
		extractor_type = "BRIEF";
		break;
    case extractor::BRISK:
		extractor_type = "BRISK";
		break;
    case extractor::ORB:
		extractor_type = "ORB";
		break;
    case extractor::FREAK:
		extractor_type = "FREAK";
		break;
	default:
        throw error_code::descriptor_extractor_not_found;
		break;
	}

	return extractor_type;
}

string Companion::get_decriptor_matcher(matcher f_matcher)
{
	string descriptor_matcher_type;

    switch (f_matcher)
	{
    case matcher::BruteForce_L2:
		descriptor_matcher_type = "BruteForce";
		break;
    case matcher::BruteForce_L1:
		descriptor_matcher_type = "BruteForce-L1";
		break;
    case matcher::BruteForce_Hamming:
		descriptor_matcher_type = "BruteForce-Hamming";
		break;
    case matcher::BruteForce_Hamming_2:
		descriptor_matcher_type = "BruteForce-Hamming(2)";
		break;
    case matcher::FlannBased:
		descriptor_matcher_type = "FlannBased";
		break;
	default:
        throw error_code::descriptor_matcher_not_found;
		break;
	}

	return descriptor_matcher_type;
}
