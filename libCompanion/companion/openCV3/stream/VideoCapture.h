//
// Created by andre on 26.11.2016.
//

#ifndef COMPANION_VIDEO_CAPTURE_H
#define COMPANION_VIDEO_CAPTURE_H

#include "opencv2/opencv.hpp"

using namespace cv;

class VideoCapture {

public:

    VideoCapture();

    ~VideoCapture();

    /* ToDo Documentation */
    int realtime_video();

};


#endif //COMPANION_VIDEO_CAPTURE_H
