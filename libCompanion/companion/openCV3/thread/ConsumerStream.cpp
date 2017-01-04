#include "ConsumerStream.h"

void ConsumerStream::run(std::string imgPath) {

    cv::Mat frame;
    cv::Mat search_img = cv::imread(imgPath, cv::IMREAD_GRAYSCALE);
    Comparison *comparison = new Comparison();
    Comparison *compare;
    comparison->setImage(search_img);

    // ToDo := Data Model to setup configuration.

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

    // https://stackoverflow.com/questions/28024048/how-to-get-efficient-result-in-orb-using-opencv-2-4-9
    // ORB cv::ORB::create(500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);
    //int nfeatures=500, float scaleFactor=1.2f, int nlevels=8, int edgeThreshold=31,
    //int firstLevel=0, int WTA_K=2, int scoreType=ORB::HARRIS_SCORE, int patchSize=31, int fastThreshold=20
    //cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create(2000, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);
    //cv::Ptr<cv::DescriptorExtractor> extractor = cv::ORB::create(2000, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);

    // Intresting
    cv::Ptr<cv::FeatureDetector> detector = cv::BRISK::create(60);
    cv::Ptr<cv::DescriptorExtractor> extractor = cv::BRISK::create(60);

    /*
        BruteForce (it uses L2 )
        BruteForce-L1
        BruteForce-Hamming
        BruteForce-Hamming(2)
        FlannBased
    */
    std::string type = "BruteForce-Hamming(2)";
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(type);
    ImageRecognition *recognition = new FeatureMatching(
            detector,
            extractor,
            matcher,
            type);

    while (true) {
        while (queue.pop(frame)) {
            if (!frame.empty()) {
                compare = new Comparison();
                // https://antifreezedesign.wordpress.com/2011/05/13/permutations-of-1920x1080-for-perfect-scaling-at-1-77/
                Util::resize_image(frame, 1024, 576);
                compare->setImage(frame);
                recognition->algo(compare, comparison);
                cv::waitKey(1);
                delete compare;
            }
        }
    }
}