//
// Created by andre on 26.11.2016.
//

#ifndef COMPANION_VIDEO_CAPTURE_H
#define COMPANION_VIDEO_CAPTURE_H

#include <opencv2/opencv.hpp>
#include <companion/openCV3/search/ImageRecognition.h>
#include <companion/openCV3/detection/HarrisCorner.h>

class Video {

public:

    Video();

    ~Video();

    int connectToDevice(int device);

    int playVideo(std::string url);

    cv::Mat obtainImage();

private:

    cv::VideoCapture capture;

    bool isDeviceOpen();

};


#endif //COMPANION_VIDEO_CAPTURE_H
