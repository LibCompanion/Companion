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

#include <companion/Configuration.h>
#include <companion/processing/2D/ObjectDetection.h>
#include <companion/algo/cuda/CudaFeatureMatching.h>
#include <companion/algo/cpu/CPUFeatureMatching.h>
#include <companion/input/Video.h>
#include <companion/input/Image.h>

#include <thread>

void callback(std::vector<std::pair<Companion::Draw::Drawable*, int>> objects, cv::Mat frame) {
    Companion::Draw::Drawable *drawable;

    for(unsigned long x = 0; x < objects.size(); x++) {
        
        // Mark the detected object
        drawable = objects.at(x).first;
        drawable->draw(frame);

        // Draw the id of the detected object
        Companion::Draw::Lines *lines = dynamic_cast<Companion::Draw::Lines*>(drawable);
        if (lines != nullptr) {
            cv::putText(frame, std::to_string(objects.at(x).second), lines->getLines()[0]->getEnd(), cv::FONT_HERSHEY_DUPLEX, 2, cv::Scalar(0, 255, 0), 4);
        }
    }

    cv::imshow("Object detection", frame);
    cv::waitKey(1);
    frame.release();
    objects.clear();
}

void sampleImageThread(Companion::Input::Image *stream) {

    bool finished = false;
    // Setup example for stream images.
    while (!finished) {

        // Use this control to adjust the streaming rate
        //cvWaitKey(300);

        for(int i = 1; i <= 432; i++ ) {
            std::string fileNr;
            if(i < 10) {
                fileNr = "000" + std::to_string(i);
            } else if(i < 100) {
                fileNr = "00" + std::to_string(i);
            } else {
                fileNr = "0" + std::to_string(i);
            }
            stream->addImage("/home/asekulsk/Dokumente/Master/Testcase/HBF/Img/hbf" + fileNr + ".jpg");
        }

        finished = true;
    }
}

void error(Companion::Error::Code code) {
    // Obtain detailed error message from code
    std::cout << Companion::Error::getError(code) << "\n";
}

int main() {

    // ToDo := Add sample directory files and build sample with expecting cmake setup if cuda on or not.

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
    Companion::Configuration *companion = new Companion::Configuration();
    int type = cv::DescriptorMatcher::BRUTEFORCE_HAMMING;
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(type);

    // -------------- BRISK CPU FM --------------
    cv::Ptr<cv::BRISK> feature = cv::BRISK::create(60);
    Companion::Algorithm::ImageRecognition *recognition = new Companion::Algorithm::CPU::FeatureMatching(feature, feature, matcher, type, 40, true);

    // -------------- ORB CPU FM --------------
    //CPU feature matching implementation.
    //cv::Ptr<cv::ORB> feature = cv::ORB::create(2000);
    //Companion::Algorithm::ImageRecognition *recognition =  new Companion::Algorithm::CPU::FeatureMatching(feature, feature, matcher, type);

    // -------------- ORB GPU FM - Needs CUDA --------------
    //cv::Ptr<cv::cuda::ORB> feature = cv::cuda::ORB::create(6000);
    //feature->setBlurForDescriptor(true);
    //Companion::Algorithm::ImageRecognition *recognition = new Companion::Algorithm::Cuda::FeatureMatching(feature);

    // -------------- Image Processing Setup --------------
    companion->setProcessing(new Companion::Processing::ObjectDetection(companion, recognition, 0.50));
    companion->setSkipFrame(0);
    companion->setResultHandler(callback);
    companion->setErrorHandler(error);

    // Setup video source to obtain images.
    //Companion::Input::Stream *stream = new Companion::Input::Video(testVideo); // Load an video
    //Companion::Input::Stream *stream = new Companion::Input::Video(0); // Realtime input

    // Setup example for an streaming data from a set of images.
    Companion::Input::Image *stream = new Companion::Input::Image(50);
    std::thread imgThread = std::thread(&sampleImageThread, stream);

    // Set input source
    companion->setSource(stream);

    // Store all searched data models
    Companion::Model::FeatureMatchingModel *object;
    for (int i = 0; i < images.size(); i++) {
        object = new Companion::Model::FeatureMatchingModel();
        object->setID(i);
        object->setImage(cv::imread(images[i], cv::IMREAD_GRAYSCALE));

        // Only works on CPU -- ToDo Exception Handling if wrong type?
        //object->calculateKeyPointsAndDescriptors(feature, feature);

        if(!companion->addModel(object)) {
            std::cout << "Model not added";
        }
    }

    // Companion class to execute algorithm
    std::queue<cv::Mat> queue;
    Companion::Thread::StreamWorker ps(queue);

    try {
        companion->run(ps);
        //imgThread.join(); // External img thread to add images by processing.
    } catch (Companion::Error::Code errorCode) {
        error(errorCode);
    }

    return 0;
}