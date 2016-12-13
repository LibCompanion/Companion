//
// Created by asekulsk on 13.12.16.
//

#include "ConsumerStream.h"

ConsumerStream::ConsumerStream(wqueue<cv::Mat> &queue) {
    this->queue = queue;
}

void ConsumerStream::run(std::string imgPath) {

    cv::Mat frame;
    cv::Mat search_img = cv::imread(imgPath, cv::IMREAD_GRAYSCALE);
    cv::Ptr<cv::FeatureDetector> detector = cv::BRISK::create();
    cv::Ptr<cv::DescriptorExtractor> extractor = cv::BRISK::create();
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
    ImageRecognition *recognition = new FeatureMatching(
            detector,
            extractor,
            matcher);

    while (true) {
        if(queue.size() > 0) {
            frame = queue.remove();
            if(!frame.empty()) {
                imshow("Good Matches & Object detection", frame);
                //        recognition->search(search_img, frame);
            }
        } else {
            imshow("Good Matches & Object detection", search_img);
            cv::waitKey(1);
        }
    }

}
