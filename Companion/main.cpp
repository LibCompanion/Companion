// CompareCV.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "companion\companion.h"
#include "dirent.h"
#include <stdlib.h>

using namespace std;
using namespace cv;

int main() {

	string check_files_path = "D:/Magic_Cards_Img/All/";
	string search_file_path = "D:/Magic_Cards_Img/Test/Sub/testcard1.jpg";

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

	// Compare matching - Fast but results can vary
	Compare image = companion.search_compare_image_mp(search_file_path, card_images, 0.05);
	
	// Template matching - Moderrate result good
	// http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/template_matching/template_matching.html
	// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	// CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED
	//TemplateMatch image = companion.search_template_matching_mp(search_file_path, card_images, 0.8 , CV_TM_CCORR_NORMED);

	// Flann matching - Slowest but best results
	// Requires OpenCV NonFree libs... be careful !!
	// http://docs.opencv.org/2.4/modules/nonfree/doc/nonfree.html
	// http://docs.opencv.org/2.4/doc/tutorials/features2d/feature_flann_matcher/feature_flann_matcher.html
	//Flann image = companion.search_flann_mp(search_file_path, card_images, 0.1);

	if (!image.get_compare_image_path().empty()) {
		std::cout << "Accordance : " << image.get_accordance() << "\n";
		image.show_images();
	}
	else
	{
		std::cout << "No image found\n";
	}
	
	std::system("pause");

	return 0;
}