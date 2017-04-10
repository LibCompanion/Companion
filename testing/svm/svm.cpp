#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv/ml.h>
#include <opencv/cv.hpp>

int main( int argc, const char** argv ) {

    // https://www.codeproject.com/Articles/619039/Bag-of-Features-Descriptor-on-SIFT-Features-with-O

    // Step 1 Obtain the set of bags of features.
    // Feature detector and descriptor
    cv::Ptr<cv::Feature2D> algo = cv::xfeatures2d::SIFT::create();
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("FlannBased");

    cv::TermCriteria tc(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 10, 0.001);
    int directorySize = 4 * 60;
    int retries = 1;
    int flags = cv::KMEANS_PP_CENTERS;

    cv::BOWKMeansTrainer bowTrainer(directorySize, tc, retries, flags);

    // Training data from four types Tiger, Airplane, Bike and Car.
    // Name from class img are <Class> (Nr_IMG).jpg
    // http://ttic.uchicago.edu/~mostajabi/Tutorial.html
    cv::Mat img;
    std::string file = "/home/asekulsk/Dokumente/Master/Testcase/SVM/FourClass/train/";
    for(int j=1; j<=4; j++) {
        for (int i = 1; i <= 60; i++) {
            cv::Mat features;
            std::vector <cv::KeyPoint> keypoints;

            img = cv::imread(file + std::to_string(j) + " (" + std::to_string(i) + ").jpg", 0);

            algo->detect(img, keypoints);
            algo->compute(img, keypoints, features);
            bowTrainer.add(features);
            img.release();
        }
    }

    // ToDo := Difference between dictionary and next code.
    cv::Mat dictionary = bowTrainer.cluster();

    // ToDo := I don't have a clue how it works and what i do her...
    // Step 2 Obtain BOF descriptor for given image/video frame.
    cv::Mat bowDescriptor;
    cv::Mat labels(0, 1, CV_32FC1);
    cv::Mat trainingData(0, directorySize, CV_32FC1);
    cv::BOWImgDescriptorExtractor bowDE(algo, matcher);
    bowDE.setVocabulary(dictionary);

    for(int j=1; j<=4; j++) {
        for (int i = 1; i <= 60; i++) {
            std::vector <cv::KeyPoint> keypoints;
            img = cv::imread(file + std::to_string(j) + " (" + std::to_string(i) + ").jpg", 0);
            algo->detect(img, keypoints);
            bowDE.compute(img, keypoints, bowDescriptor);
            trainingData.push_back(bowDescriptor);
            labels.push_back(j);
            img.release();
        }
    }

    // New in opencv 3.X
    cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(trainingData, cv::ml::SampleTypes::ROW_SAMPLE, labels) ;
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->train(td);

    file = "/home/asekulsk/Dokumente/Master/Testcase/SVM/FourClass/eval/";

    float sum = 0;

    for(int j=1; j<=4; j++) {
        for(int i=1; i<=60; i++){

            std::vector <cv::KeyPoint> keypoints;

            img = cv::imread(file + std::to_string(j) + " (" + std::to_string(i) + ").jpg", 0);
            algo->detect(img, keypoints);
            bowDE.compute(img, keypoints, bowDescriptor);

            float response = svm->predict(bowDescriptor);

            sum += response;

            if(response == 1) {
                std::cout << "It's an airplane\n";
            } else if(response == 2) {
                std::cout << "It's a car\n";
            } else if(response == 3) {
                std::cout << "It's an tiger\n";
            } else if(response == 4) {
                std::cout << "It's an bike\n";
            } else {
                std::cout << "I don't have a clue what it is...";
            }

            //cv::imshow("Final", img);
            //cv::waitKey(1);

            img.release();
        }
    }

    float predicition = 60 * 1 + 60 * 2 + 60 * 3 + 60 * 4;

    std::cout << std::to_string(predicition) << " / " << std::to_string(sum);
}