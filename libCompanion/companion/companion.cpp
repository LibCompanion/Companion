#include "companion.h"

// ToDo := This class should be deleted later because all implementations should be moved to opencv3 Package!!!

void Companion::calc_histogram(std::string img_path)
{
	cv::Mat src, dst;

	/// Load image
	src = cv::imread(img_path, 1);

	if (src.empty())
	{
		return;
	}

	/// Separate the image in 3 places ( B, G and R )
	std::vector<cv::Mat> bgr_planes;
	split(src, bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	cv::Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			 cv::Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			 cv::Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			 cv::Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			 cv::Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			 cv::Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			 cv::Scalar(0, 0, 255), 2, 8, 0);
	}

	/// Display
	cv::namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	cv::imshow("calcHist Demo", histImage);

	cv::waitKey(0);
}


void Companion::search_compare_image_mp(std::string search_img_path, std::vector<std::string> compare_image_paths, double min_threshold)
{
    //Compare *best_image = new Compare();
/*
    #pragma omp parallel
	{
		// ToDo := Stop if card found with 100%
		for (auto const& check_image_path : compare_image_paths)
		{
            #pragma omp single nowait
			{
				try 
				{
                    //Compare *result = search_compare_image(search_img_path, check_image_path);

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
*/
	//return best_image;
}

void Companion::search_vector_template_matching(std::string search_img_path, std::vector<std::string> compare_img_paths, double threshold, int match_method, bool resize_same_size)
{
	/*
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
	 */
}

void Companion::search_feature_matching_mp(std::string search_img_path, std::vector<std::string> compare_img_paths, double min_threshold)
{
	/*
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
	 */
}