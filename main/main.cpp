/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2017 Andreas Sekulski
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <companion/openCV3/thread/ProducerStream.h>
#include <companion/openCV3/thread/ConsumerStream.h>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/thread.hpp>
#include <companion/openCV3/processing/ObjectDetection.h>

int main() {

    std::vector<std::string> images;

    // Windows
	std::string path = "D:/Data/Master/Testcase/HBF/";
	//std::string path = "D:/Data/Master/Testcase/UNI/";

    // Linux
    //std::string path = "/home/asekulsk/Dokumente/Master/Testcase/HBF/";
    images.push_back(path + std::string("Sample_Middle.jpg"));
    images.push_back(path + std::string("Sample_Left.jpg"));
    images.push_back(path + std::string("Sample_Right.jpg"));
    std::string testVideo = path + std::string("Muelheim_HBF.mp4");


    /*
    std::string path = "/home/asekulsk/Dokumente/Master/Testcase/UNI/";
    images.push_back(path + std::string("karl.PNG"));
    images.push_back(path + std::string("max.PNG"));
    images.push_back(path + std::string("hertz.jpg"));
    images.push_back(path + std::string("roentgen.jpg"));
    std::string testVideo = path + std::string("UG.mp4");
    */

    /*
    std::string path = "/home/asekulsk/Dokumente/Master/Testcase/IFIS/";
    images.push_back(path + std::string("left.jpg"));
    images.push_back(path + std::string("right.jpg"));
    std::string testVideo = path + std::string("info.mp4");
    */

    CompanionConfig *config = new CompanionConfig();

    // Setup used processing algo.

    // https://stackoverflow.com/questions/28024048/how-to-get-efficient-result-in-orb-using-opencv-2-4-9
    // ORB cv::ORB::create(500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);

    //int nfeatures=500, float scaleFactor=1.2f, int nlevels=8, int edgeThreshold=31,
    //int firstLevel=0, int WTA_K=2, int scoreType=ORB::HARRIS_SCORE, int patchSize=31, int fastThreshold=20
    //cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create(1500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);
    //cv::Ptr<cv::DescriptorExtractor> extractor = cv::ORB::create(1500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);

	cv::Ptr<cv::BRISK> brisk = cv::BRISK::create(60);
    std::string type = "BruteForce-Hamming(2)";
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(type);

    FeatureMatching *recognition = new FeatureMatching(
			brisk,
			brisk,
            matcher,
            type);

    config->setProcessing(new ObjectDetection(config, recognition));

    // Generate video source
    Video *video = new Video();
    video->playVideo(testVideo);
    config->setSource(video);

    // Store all searched data models
    FeatureMatchingModel *object;
    for (auto &image : images) {
        object = new FeatureMatchingModel();
        object->setImage(cv::imread(image, cv::IMREAD_GRAYSCALE));
        config->addModel(object);
    }

    // Companion class to execute algorithm
    boost::lockfree::spsc_queue<cv::Mat> queue(30);
    ProducerStream ps(queue);
    ConsumerStream cs(queue);

    boost::thread t1(boost::bind(&ProducerStream::run, &ps, config));
    boost::thread t2(boost::bind(&ConsumerStream::run, &cs, config));
    t1.join();
    t2.join();

    return 0;
}