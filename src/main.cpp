#include "companion.h"
#include <stdlib.h>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void out_message(string header, Search *search) {
    if (!search->get_compare_image_path().empty()) {
        cout << header << "\n" << "Accordance : " << search->get_accordance() << "\n";
        cout << "Search Image : " << search->get_search_image_path() << "\n";
        cout << "Compare Image : " << search->get_compare_image_path() << "\n";
	}
	else
	{
		std::cout << header << "\n" << "No image found\n";
	}
}

void compare_matching(string seach_file_path_name, vector<string> card_images, Companion companion) {

	string search_file_path; 
	int start_s;
	Search *image;
	int stop_s;

	// Compare matching - Fast but results can vary
	for (int i = 1; i <= 5; i++)
	{
		search_file_path = seach_file_path_name + to_string(i) + ".jpg";
		start_s = clock();
		image = companion.search_compare_image_mp(search_file_path, card_images, 0.05);
		stop_s = clock();
		out_message("Simple_Compare", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
	}
}

void template_matching(string seach_file_path_name, vector<string> card_images, Companion companion)
{
	string search_file_path;
	int start_s;
	Search *image;
	int stop_s;

	// Template matching - Moderrate result good
	// http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/template_matching/template_matching.html
	// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	// CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED

	for (int i = 1; i <= 5; i++)
	{
		search_file_path = seach_file_path_name + to_string(i) + ".jpg";
		start_s = clock();
		image = companion.search_vector_template_matching(search_file_path, card_images, 0.20, CV_TM_SQDIFF_NORMED, true);
		stop_s = clock();
		out_message("Template_Matching_CV_TM_SQDIFF_NORMED", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
	}


	for (int i = 1; i <= 5; i++)
	{
		search_file_path = seach_file_path_name + to_string(i) + ".jpg";
		start_s = clock();
		image = companion.search_vector_template_matching(search_file_path, card_images, 0.80, CV_TM_CCORR_NORMED, true);
		stop_s = clock();
		out_message("Template_Matching_CV_TM_CCORR_NORMED", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
	}

	for (int i = 1; i <= 5; i++)
	{
		search_file_path = seach_file_path_name + to_string(i) + ".jpg";
		start_s = clock();
		image = companion.search_vector_template_matching(search_file_path, card_images, 0.80, CV_TM_CCOEFF_NORMED, true);
		stop_s = clock();
		out_message("Template_Matching_CV_TM_CCOEFF_NORMED", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
	}
}

void flann_matching(string seach_file_path_name, vector<string> card_images, Companion companion)
{
	string search_file_path;
	int start_s;
	Flann *image;
	int stop_s;

	// Flann matching - Slowest but best results
	// Requires OpenCV NonFree libs... be careful !!
	// http://docs.opencv.org/2.4/modules/nonfree/doc/nonfree.html
	// http://docs.opencv.org/2.4/doc/tutorials/features2d/feature_flann_matcher/feature_flann_matcher.html

	for (int i = 1; i <= 5; i++)
	{
		search_file_path = seach_file_path_name + to_string(i) + ".jpg";
		start_s = clock();
		image = companion.search_flann_mp(search_file_path, card_images, 0.1);
		stop_s = clock();
		out_message("FLANN", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
		image->show_compare_points();
	}
}

int main() {

    string check_files_path = "D:/Magic_Cards_Img/data_b.txt";
    string seach_file_path_name = "D:/Magic_Cards_Img/Test/testcard";
	string search_file_path;

	vector<string> card_images;
    Companion companion;

	string path;
	ifstream myfile(check_files_path);
	if (myfile.is_open())
	{
		while (getline(myfile, path))
		{
			card_images.push_back(path);
		}
		myfile.close();
	}
	else
	{
		cout << "Testdata not found\n";
		system("pause");
		return 0;
	}


	string test_img_template = "D:/Magic_Cards_Img/Test/Template/template.jpg";
	string test_img_path = "D:/Magic_Cards_Img/Test/Template/inverse.jpg";
	try
	{
		//Search *search = companion.search_flann(test_img_path, test_img_template);
		TemplateMatch *search = companion.search_template_matching(test_img_path, test_img_template, CV_TM_CCOEFF_NORMED, false);
		cout << search->get_accordance() << "\n";
        search->show_compare_points();
	}
	catch (Error e)
	{
		// ToDo := Better Error Handling
		cout << companion.get_error(e) << "\n";
	}
	

	//companion.calc_histogram(seach_file_path_name + to_string(4) + ".jpg");
	//compare_matching(seach_file_path_name, card_images, companion);
	//template_matching(seach_file_path_name, card_images, companion);
	//flann_matching(seach_file_path_name, card_images, companion);
	
	return 0;
}
