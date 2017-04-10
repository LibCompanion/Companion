#include "Video.h"

Video::Video(int device) {

    cv::VideoCapture cap(device);

    if (!cap.isOpened()) {
        throw CompanionError::errorCode::invalid_video_src;
    }

    capture = cap;
}


Video::Video(std::string url) {

    cv::VideoCapture cap(url);

    if (!cap.isOpened()) {
        throw CompanionError::errorCode::invalid_video_src;
    }

    capture = cap;
}

Video::~Video() {
    // No need to close capture device because closing will be done from VideoCapture destructor.
}

cv::Mat Video::obtainImage() {

    cv::Mat frame;

    if (!capture.isOpened()) {
        return frame;
    }

    // Obtain image frame.
    capture >> frame;

    return frame;
}