#include "Video.h"

Video::Video() {
}

Video::~Video() {
    // No need to close capture device because closing will be done from VideoCapture destructor.
}

int Video::connectToDevice(int device) {

    // ToDo := check if an video device is already running.

    cv::VideoCapture cap(device);

    // ToDo Method
    if (!cap.isOpened()) {
        // ToDo := Error Handling
        return -1;
    }

    capture = cap;

    return 0;
}

int Video::playVideo(std::string url) {

    // ToDo := check if an video device is already running.

    cv::VideoCapture cap(url);

    // ToDo Method
    if (!cap.isOpened()) {
        // ToDo := Error Handling
        return -1;
    }

    capture = cap;

    return 0;
}

cv::Mat Video::obtainImage() {

    cv::Mat frame;

    if (!capture.isOpened()) {
        // ToDo := Error Handling
        return frame;
    }

    // Obtain image frame.
    capture >> frame;
    return frame;
}