#include <stdlib.h>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>

#include <companion/companion.h>
#include <companion/openCV3/stream/Video.h>
#include <companion/openCV3/search/ImageRecognition.h>
#include <companion/openCV3/search/SimpleCompareImage.h>
#include <companion/openCV3/search/TemplateMatch.h>
#include <companion/openCV3/search/FeatureMatching.h>
#include <companion/openCV3/detection/ObjectTracking.h>
#include <companion/openCV3/thread/ProducerStream.h>
#include <companion/openCV3/thread/ConsumerStream.h>

int main() {

	Video video;
	//string testImg = "/home/asekulsk/Bilder/Magic_Cards_Img/Test/testcard1.jpg";
	//string testImg = "D:/Data/Magic_Cards_Img/Test/testcard1.jpg";
	std::string path = "/media/asekulsk/F898585998581908/Data/Master/Testcase/HBF/";
    std::string testImg = path + std::string("Sample_Right.jpg");
    std::string testVideo = path + std::string("Muelheim_HBF.mp4");

    wqueue<cv::Mat> queue;
    ProducerStream ps(queue);
    ConsumerStream cs(queue);

    std::thread t1(&ProducerStream::run, ps, testVideo);
    std::thread t2(&ConsumerStream::run, cs, testImg);
    t1.join();
    t2.join();

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
        cv::Ptr<cv::FeatureDetector> detector = cv::BRISK::create();
        cv::Ptr<cv::DescriptorExtractor> extractor = cv::BRISK::create();

        /*
        BruteForce (it uses L2 )
        BruteForce-L1
        BruteForce-Hamming
        BruteForce-Hamming(2)
        FlannBased
        */
        cv::Ptr<cv::DescriptorMatcher> matcher;
		matcher = cv::DescriptorMatcher::create("BruteForce");
		//matcher = DescriptorMatcher::create("BruteForce-L1");
		//matcher = DescriptorMatcher::create("BruteForce-Hamming");
		//matcher = DescriptorMatcher::create("BruteForce-Hamming(2)");
		//matcher = DescriptorMatcher::create("FlannBased");

		ImageRecognition *recognition = new FeatureMatching(
                detector,
                extractor,
                matcher);

        /*
		Mat frame;
		//video.connectToDevice(0);
        video.playVideo(testVideo);
        // Camera api
        frame = video.obtainImage();
		while(!frame.empty()) {
			// Image recognition
			cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
			recognition->search(imread(testImg, IMREAD_GRAYSCALE), frame);
			// Wait to obtain next image or store to buffer
			waitKey(1);
            frame = video.obtainImage();
		}
        */

        //if(video.startRealtime(recognition, imread(testcard, IMREAD_GRAYSCALE), 0) != 0); {
        //    cout << "Scotty we have a problem";
        //}

		// Intresting maybe could help...
		// https://www.youtube.com/watch?v=bSeFrPrqZ2A
		//ObjectTracking *tracking = new ObjectTracking();
		//tracking->start();

		//delete tracking;
        delete recognition;

    } catch (CompanionError::error_code error) {
        std::cout << Util::get_error(error);
    }

	return 0;
}
