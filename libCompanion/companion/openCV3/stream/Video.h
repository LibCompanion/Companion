//
// Created by andre on 26.11.2016.
//

#ifndef COMPANION_VIDEO_CAPTURE_H
#define COMPANION_VIDEO_CAPTURE_H

#include <opencv2/opencv.hpp>
#include <companion/openCV3/search/ImageRecognition.h>
#include <companion/openCV3/detection/HarrisCorner.h>

using namespace cv;

class Video {

public:

    Video();

    ~Video();

    int connectToDevice(int device);

    Mat obtainImage();

private:

    VideoCapture capture;

    bool isDeviceOpen();

};


#endif //COMPANION_VIDEO_CAPTURE_H
