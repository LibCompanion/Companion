#include <companion/openCV3/detection/RectangleDetection.h>
#include "ConsumerStream.h"

void ConsumerStream::run(std::vector<std::string> images) {

    cv::Mat frame;
    ImageRecognitionModel *object;
    ImageRecognitionModel *scene;
    Drawable *drawable;
    std::vector<ImageRecognitionModel*> sImages;

    for (auto &image : images) {
        object = new FeatureMatchingModel();
        object->setImage(cv::imread(image, cv::IMREAD_GRAYSCALE));
        sImages.push_back(object);
    }

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
    //cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create(1500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);
    //cv::Ptr<cv::DescriptorExtractor> extractor = cv::ORB::create(1500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);

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

    RectangleDetection *detection = new RectangleDetection();

    while (true) {
		
        while (queue.pop(frame)) {
            if (!frame.empty()) {
                scene = new FeatureMatchingModel();
                // https://antifreezedesign.wordpress.com/2011/05/13/permutations-of-1920x1080-for-perfect-scaling-at-1-77/
                Util::resizeImage(frame, 1024, 576);
                scene->setImage(frame);

                // ToDo := Multiple Sub Methods

                detection->detect(frame);


/*
                #pragma omp parallel for
                for(int x = 0; x < sImages.size(); x++) {
                    drawable = recognition->algo(scene, sImages.at(x));
                    if(drawable != nullptr) {
                        drawable->draw(frame);
                    }
                }
*/
                cv::imshow("Object detection", frame);
                

                cv::waitKey(0);
                delete scene;
            }
        }
		
    }
}