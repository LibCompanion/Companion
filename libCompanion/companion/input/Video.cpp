#include "Video.h"

Companion::Input::Video::Video(int device) {

    cv::VideoCapture cap(device);

    if (!cap.isOpened()) {
        throw Companion::Error::Code::invalid_video_src;
    }

    capture = cap;
}


Companion::Input::Video::Video(std::string url) {

    cv::VideoCapture cap(url);

    if (!cap.isOpened()) {
        throw Companion::Error::Code::invalid_video_src;
    }

    capture = cap;
}

Companion::Input::Video::~Video() {
    // No need to close capture device because closing will be done from VideoCapture destructor.
}

cv::Mat Companion::Input::Video::obtainImage() {

    cv::Mat frame;

    if (!capture.isOpened()) {
        return frame;
    }

    // Obtain image frame.
    capture >> frame;

    return frame;
}