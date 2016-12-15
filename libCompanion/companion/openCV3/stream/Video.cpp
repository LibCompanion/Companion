#include "Video.h"

Video::Video() {
}

Video::~Video() {
}

int Video::connectToDevice(int device) {

    cv::VideoCapture cap(device);

    // ToDo Method
    if(!cap.isOpened()) {
        // ToDo := Error Handling
        return -1;
    }

    capture = cap;

    return 0;
}

int Video::playVideo(std::string url) {

    cv::VideoCapture cap(url);

    // ToDo Method
    if(!cap.isOpened()) {
        // ToDo := Error Handling
        return -1;
    }

    capture = cap;

    return 0;
}

cv::Mat Video::obtainImage() {

    cv::Mat frame;

    if(!capture.isOpened()) {
        // ToDo := Error Handling
    }

    // Obtain image frame.
    capture >> frame;
    return frame;
}