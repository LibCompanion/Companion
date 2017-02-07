#include "Video.h"

Video::Video() {

}

Video::~Video() {
    // No need to close capture device because closing will be done from VideoCapture destructor.
}

void Video::connectToDevice(int device) {

    cv::VideoCapture cap(device);
    if (!cap.isOpened()) {
        throw CompanionError::errorCode::invalid_video_src;
    }

    capture = cap;
}

void Video::playVideo(std::string url) {

    cv::VideoCapture cap(url);
    if (!cap.isOpened()) {
        throw CompanionError::errorCode::invalid_video_src;
    }

    capture = cap;
}

cv::Mat Video::obtainImage() {

    cv::Mat frame = cv::Mat::zeros(cv::Size(1,1), CV_8UC3);

    if (!capture.isOpened()) {
        return frame;
    }

    // Obtain image frame.
    capture >> frame;
    return frame;
}

bool Video::isOpen() {
    capture.isOpened();
}