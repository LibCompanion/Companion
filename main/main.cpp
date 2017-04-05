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

#include <companion/Companion.h>
#include <companion/processing/2D/ObjectDetection.h>
#include <companion/algo/cuda/CudaFeatureMatching.h>
#include <companion/algo/cpu/CPUFeatureMatching.h>
#include <companion/stream/Video.h>
#include <companion/stream/Image.h>

void callback(std::vector<Drawable*> objects, cv::Mat frame) {
    Drawable *drawable;

    for(int x = 0; x < objects.size(); x++) {
        drawable = objects.at(x);
        drawable->draw(frame);
    }

    cv::imshow("Object detection", frame);
    cv::waitKey(1);
    frame.release();
}

void error(CompanionError::errorCode code) {
    // Obtain detailed error message from code
    std::cout << CompanionError::getError(code) << "\n";
}

int main() {

    std::vector<std::string> images;

    // NPA Example - Only Realtime
    /*
    std::string path = "/home/asekulsk/Dokumente/Master/Testcase/NPA_REAL/";
    images.push_back(path + std::string("Andy.jpg"));
    //images.push_back(path + std::string("Black_Andy.jpg"));
    */

    // Perfomance increase are ProducerConsumer Design Pattern (Libbost), Frame skipping and Cuda

    //std::string path = "D:/Data/Master/Testcase/HBF/";
    std::string path = "/home/asekulsk/Dokumente/Master/Testcase/HBF/";
    images.push_back(path + std::string("Sample_Middle.jpg"));
    images.push_back(path + std::string("Sample_Left.jpg"));
    images.push_back(path + std::string("Sample_Right.jpg"));
    std::string testVideo = path + std::string("Muelheim_HBF.mp4");


    // ToDo -> Detection
    /*
    //std::string path = "D:/Data/Master/Testcase/UNI/";
    std::string path = "/home/asekulsk/Dokumente/Master/Testcase/UNI/";
    images.push_back(path + std::string("karl.PNG"));
    images.push_back(path + std::string("max.PNG"));
    images.push_back(path + std::string("hertz.jpg"));
    images.push_back(path + std::string("roentgen.jpg"));
    std::string testVideo = path + std::string("UG.mp4");
    */

    // ToDo -> ImageRecognitionModel Bug
    // ToDo := Area bug in model
    /*
    //std::string path = "D:/Data/Master/Testcase/IFIS/";
    std::string path = "/home/asekulsk/Dokumente/Master/Testcase/IFIS/";
    images.push_back(path + std::string("left.jpg"));
    images.push_back(path + std::string("right.jpg"));
    std::string testVideo = path + std::string("info.mp4");
    */

    // -------------- Setup used processing algo. --------------
    Companion *companion = new Companion();
    int type = cv::DescriptorMatcher::BRUTEFORCE_HAMMING;
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(type);

    // -------------- BRISK CPU FM --------------
    //cv::Ptr<cv::BRISK> feature = cv::BRISK::create(60);
    //ImageRecognition *recognition = new CPUFeatureMatching(feature, feature, matcher, type, 40, true);

    // -------------- ORB CPU FM --------------
    //CPU feature matching implementation.
    //cv::Ptr<cv::ORB> feature = cv::ORB::create(2000);
    //ImageRecognition *recognition = new CPUFeatureMatching(feature, feature, matcher, type);

    // -------------- ORB GPU FM - Needs CUDA --------------
    cv::Ptr<cv::cuda::ORB> feature = cv::cuda::ORB::create(6000);
    feature->setBlurForDescriptor(true);
    ImageRecognition *recognition = new CudaFeatureMatching(feature);

    // -------------- Image Processing Setup --------------
    companion->setProcessing(new ObjectDetection(companion, recognition, 0.50));
    companion->setSkipFrame(0);
    companion->setResultHandler(callback);
    companion->setErrorHandler(error);

    // Setup video source to obtain images.
    Stream *stream = new Video(testVideo); // Load an video
    //Stream *stream = new Video(0); // Realtime stream

    // Setup example for an streaming data from images.
    /*
    Image *stream = new Image();
    for(int i = 1; i <= 432; i++ ) {
        std::string fileNr;
        if(i < 10) {
            fileNr = "000" + std::to_string(i);
        } else if(i < 100) {
            fileNr = "00" + std::to_string(i);
        } else {
            fileNr = "0" + std::to_string(i);
        }
        stream->addImagePath("/home/asekulsk/Dokumente/Master/Testcase/HBF/Img/hbf" + fileNr + ".jpg");
    }
    */

    companion->setSource(stream);

    // Store all searched data models
    FeatureMatchingModel *object;
    for (auto &image : images) {
        object = new FeatureMatchingModel();
        object->setImage(cv::imread(image, cv::IMREAD_GRAYSCALE));

        // Only works on CPU -- ToDo Exception Handling if wrong type?
        //object->calculateKeyPointsAndDescriptors(feature, feature);

        if(!companion->addModel(object)) {
            std::cout << "Model not added";
        }
    }

    // Companion class to execute algorithm
    std::queue<cv::Mat> queue;
    StreamWorker ps(queue);

    try {
        companion->run(ps);
    } catch (CompanionError::errorCode errorCode) {
        error(errorCode);
    }


    return 0;
}