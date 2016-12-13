//
// Created by asekulsk on 13.12.16.
//

#include "ConsumerStream.h"

void ConsumerStream::run(std::string imgPath) {

    cv::Mat frame;
    cv::Mat search_img = cv::imread(imgPath, cv::IMREAD_GRAYSCALE);

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
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
    ImageRecognition *recognition = new FeatureMatching(
            detector,
            extractor,
            matcher);

    while (true) {
        frame = queue.remove();
        if(!frame.empty()) {
            // https://antifreezedesign.wordpress.com/2011/05/13/permutations-of-1920x1080-for-perfect-scaling-at-1-77/
            Util::resize_image(frame, 1280, 720);
            recognition->search(search_img, frame);
            cv::waitKey(1);
        }
    }

}
