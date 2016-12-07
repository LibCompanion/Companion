#include <stdlib.h>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>

#include <companion/companion.h>
#include <companion/openCV3/stream/Video.h>
#include <companion/openCV3/search/ImageRecognition.h>
#include <companion/openCV3/search/SimpleCompareImage.h>
#include <companion/openCV3/search/TemplateMatch.h>
#include <companion/openCV3/search/FeatureMatching.h>

using namespace std;

/*
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
 */

void compare_matching(string seach_file_path_name, vector<string> card_images, vector<string> test_cards, Companion companion) {

	string search_file_path; 
	int start_s;
	//Search *image;
	int stop_s;

	// Compare matching - Fast but results can vary
	for (int i = 0; i < test_cards.size(); i++)
	{
		search_file_path = test_cards.at(i);
		start_s = clock();
		//image = companion.search_compare_image_mp(search_file_path, card_images, 0.05);
		stop_s = clock();
		//out_message("Simple_Compare", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
		//delete image;
	}
}

void template_matching(string seach_file_path_name, vector<string> card_images, vector<string> test_cards, Companion companion)
{
	string search_file_path;
	int start_s;
	//Search *image;
	int stop_s;

	// Template matching - Moderrate result good
	// http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/template_matching/template_matching.html
	// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	// CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED

	for (int i = 0; i < test_cards.size(); i++)
	{
		search_file_path = test_cards.at(i);
		start_s = clock();
		//image = companion.search_vector_template_matching(search_file_path, card_images, 0.20, CV_TM_SQDIFF_NORMED, true);
		stop_s = clock();
		//out_message("Template_Matching_CV_TM_SQDIFF_NORMED", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
		//delete image;
	}


	for (int i = 0; i < test_cards.size(); i++)
	{
		search_file_path = test_cards.at(i);
		start_s = clock();
		//image = companion.search_vector_template_matching(search_file_path, card_images, 0.80, CV_TM_CCORR_NORMED, true);
		stop_s = clock();
		//out_message("Template_Matching_CV_TM_CCORR_NORMED", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
		//delete image;
	}

	for (int i = 0; i < test_cards.size(); i++)
	{
		search_file_path = test_cards.at(i);
		start_s = clock();
		//image = companion.search_vector_template_matching(search_file_path, card_images, 0.80, CV_TM_CCOEFF_NORMED, true);
		stop_s = clock();
		//out_message("Template_Matching_CV_TM_CCOEFF_NORMED", image);
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
		//delete image;
	}
}

void feature_matching(string seach_file_path_name, vector<string> card_images, vector<string> test_cards, Companion companion)
{
	string search_file_path;
	int start_s;
	//FeatureMatch *image;
	int stop_s;

	for (int i = 0; i < test_cards.size(); i++)
	{
		//for (int z = 0; z < 5; z++)
		{
			search_file_path = test_cards.at(i);
			start_s = clock();
			//image = companion.search_feature_matching_mp(
			//	search_file_path,
			//	card_images,
			//	0.1,
            //    Companion::detector::ORB,
            //    Companion::extractor::ORB,
            //    Companion::matcher::BruteForce_L2);
			stop_s = clock();
			//out_message("Feature Matching", image);
			cout << " time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << "\n";
			//image->show_compare_points();
			//delete image;
		}
		
	}
}

int main() {

	Video video;
	string testcard = "/home/asekulsk/Bilder/Room/1998.jpg";

    // ToDo all totally changed...
    // New version from image recognition companion lib...
    try {
        // https://stackoverflow.com/questions/36691050/opencv-3-list-of-available-featuredetectorcreate-and-descriptorextractorc
        /*
        BRISK: detector + descriptor
        ORB: detector + descriptor
        MSER: detector
        FAST: detector
        AGAST: detector
        GFFT: detector
        SimpleBlobDetector: detector
        KAZE: detector + descriptor
        AKAZE: detector + descriptor
        FREAK: descriptor
        StarDetector: detector
        BriefDescriptorExtractor: descriptor
        LUCID: descriptor
        LATCH: descriptor
        DAISY: descriptor
        MSDDetector: detector
        SIFT: detector + descriptor (NonFree)
        SURF: detector + descriptor (NonFree)
        */
        Ptr<FeatureDetector> detector = ORB::create();
        Ptr<DescriptorExtractor> extractor = ORB::create();

        /*
        BruteForce (it uses L2 )
        BruteForce-L1
        BruteForce-Hamming
        BruteForce-Hamming(2)
        FlannBased
        */
        Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");

		ImageRecognition *recognition = new FeatureMatching(
                detector,
                extractor,
                matcher);

        if(video.startRealtime(recognition, imread(testcard, IMREAD_GRAYSCALE), 1) != 0); {
            cout << "Scotty we have a problem";
        }

        delete recognition;

    } catch (CompanionError::error_code error) {
        cout << Util::get_error(error);
    }

	/*
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

	vector<string> test_cards;
	test_cards.push_back(card_image_path + "/Color_Classifier/ztest/black.jpg");
	*/
	//test_cards.push_back("D:/Magic_Cards_Img/Color_Classifier/ztest/white.jpg");

	/*
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
    */
	/*
	string img1 = card_image_path + "/Color_Classifier/ztest/black.jpg";
	string img2 = card_image_path + "/Color_Classifier/ztest/black.jpg";
	FeatureMatch *fm = companion.search_feature_matching(img1, img2, Companion::detector::FAST, Companion::extractor::ORB, Companion::matcher::BruteForce_L2);
	fm->show_images();
	fm->show_compare_points();
	*/
	//companion.calc_histogram(seach_file_path_name + to_string(4) + ".jpg");
	//compare_matching(seach_file_path_name, card_images, test_cards, companion);
	//template_matching(seach_file_path_name, card_images, test_cards, companion);
	//feature_matching(seach_file_path_name, card_images, test_cards, companion);
	
	return 0;
}
