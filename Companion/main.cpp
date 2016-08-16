// CompareCV.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "companion\companion.h"
#include "dirent.h"
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace cv;

void out_message(string header, Search search) {
	if (!search.get_compare_image_path().empty()) {
		cout << header << "\n" << "Accordance : " << search.get_accordance() << "\n";
		cout << "Search Image : " << search.get_search_image_path() << "\n";
		cout << "Compare Image : " << search.get_compare_image_path() << "\n";
		//search.show_images();
	}
	else
	{
		std::cout << header << "\n" << "No image found\n";
	}
}

int main() {

	string check_files_path = "D:/Magic_Cards_Img/Sub/";
	string seach_file_path_name = "D:/Magic_Cards_Img/Test/testcard";
	string search_file_path;

	vector<string> card_images;
	Companion companion = Companion::Companion();

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(check_files_path.c_str())) != NULL)
	{
		// print all the files and directories within directory
		while ((ent = readdir(dir)) != NULL) 
		{
			card_images.push_back(check_files_path + std::string(ent->d_name));
		}
		closedir(dir);
	}
	else 
	{
		printf("Could not open directory \n");
	}

	int start_s;
	Search image;
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

	// Template matching - Moderrate result good
	// http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/template_matching/template_matching.html
	// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	// CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED
	for (int i = 1; i <= 5; i++)
	{
		search_file_path = seach_file_path_name + to_string(i) + ".jpg";
		start_s = clock();
		image = companion.search_template_matching_mp(search_file_path, card_images, 0.20, CV_TM_SQDIFF_NORMED, true);
		stop_s = clock();
		out_message("Template_Matching_CV_TM_SQDIFF_NORMED", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
	}

	for (int i = 1; i <= 5; i++)
	{
		search_file_path = seach_file_path_name + to_string(i) + ".jpg";
		start_s = clock();
		image = companion.search_template_matching_mp(search_file_path, card_images, 0.80, CV_TM_CCORR_NORMED, true);
		stop_s = clock();
		out_message("Template_Matching_CV_TM_CCORR_NORMED", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
	}
	
	for (int i = 1; i <= 5; i++)
	{
		search_file_path = seach_file_path_name + to_string(i) + ".jpg";
		start_s = clock();
		image = companion.search_template_matching_mp(search_file_path, card_images, 0.80, CV_TM_CCOEFF_NORMED, true);
		stop_s = clock();
		out_message("Template_Matching_CV_TM_CCOEFF_NORMED", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
	}
	
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
	}
	
	std::system("pause");

	return 0;
}