//
// Created by andre on 26.11.2016.
//

#include "VideoCapture.h"

VideoCapture::video_capture() {

}

VideoCapture::~video_capture() {

}

int VideoCapture::realtime_video() {

    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened()) {
        // check if we succeeded
        return -1;
    }

    Mat edges;
    namedWindow("edges",1);
    while(true)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        if(!frame.empty()) {
            cvtColor(frame, edges, CV_BGR2GRAY);
            GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
            Canny(edges, edges, 0, 30, 3);
            imshow("edges", edges);
            waitKey(100);
        }
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}