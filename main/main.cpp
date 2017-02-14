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

#include <boost/lockfree/spsc_queue.hpp>
#include <boost/thread.hpp>
#include <companion/processing/ObjectDetection.h>
#include <companion/thread/ProducerStream.h>
#include <companion/thread/ConsumerStream.h>
#include <companion/algo/cuda/CFeatureMatching.h>

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

    // ToDo -> Perfomance
    // Perfomance increase are ProducerConsumer Design Pattern (Libbost) and Frame skipping
    //std::string path = "D:/Data/Master/Testcase/HBF/";

    std::string path = "/home/asekulsk/Dokumente/Master/Testcase/HBF/";
    images.push_back(path + std::string("Sample_Middle.jpg"));
    //images.push_back(path + std::string("Sample_Left.jpg"));
    //images.push_back(path + std::string("Sample_Right.jpg"));
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

    try {
        Companion *companion = new Companion();

        // Setup used processing algo.

        // https://stackoverflow.com/questions/28024048/how-to-get-efficient-result-in-orb-using-opencv-2-4-9
        // ORB cv::ORB::create(500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);

        //int nfeatures=500, float scaleFactor=1.2f, int nlevels=8, int edgeThreshold=31,
        //int firstLevel=0, int WTA_K=2, int scoreType=ORB::HARRIS_SCORE, int patchSize=31, int fastThreshold=20
        cv::Ptr<cv::ORB> orb = cv::ORB::create(6000);
        //cv::Ptr<cv::BRISK> brisk = cv::BRISK::create(60);
        int type = cv::DescriptorMatcher::BRUTEFORCE_HAMMING;
        cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(type);
        // ToDo := FeatureMatching setup and param configuration
        //FeatureMatching *recognition = new FeatureMatching(orb, orb, matcher, type);
        FeatureMatching *recognition = new CFeatureMatching();

        companion->setProcessing(new ObjectDetection(companion, recognition, 0.6));
        companion->setSkipFrame(2);
        companion->setResultHandler(callback);
        companion->setErrorHandler(error);

        // Generate video source
        Video *video = new Video();
        video->playVideo(testVideo); // Load an video
        //video->connectToDevice(0); // Realtime stream
        companion->setSource(video);

        // Store all searched data models
        FeatureMatchingModel *object;
        for (auto &image : images) {
            object = new FeatureMatchingModel();
            object->setImage(cv::imread(image, cv::IMREAD_GRAYSCALE));
            companion->addModel(object);
        }

        // Companion class to execute algorithm
        boost::lockfree::spsc_queue<cv::Mat> queue(1);
        ProducerStream ps(queue);
        ConsumerStream cs(queue);

        boost::thread t1(boost::bind(&ProducerStream::run, &ps, companion));
        boost::thread t2(boost::bind(&ConsumerStream::run, &cs, companion));
        t1.join();
        t2.join();

    } catch (CompanionError::errorCode errorCode) {
        error(errorCode);
    }

    return 0;
}