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

    /* ToDo Documentation */
    int realtime(ImageRecognition *algo, Mat img);

};


#endif //COMPANION_VIDEO_CAPTURE_H
