#include "Video.h"

Video::Video() {
}

Video::~Video() {
}

/*
int Video::startRealtime(ImageRecognition *algo, Mat img, int device) {

    Comparison *comparison;
    Point loc;
    Point offset;
    VideoCapture cap(device); // Connect to camera device
    Mat frame, grey_frame; // Stores obtained images

    if(isRecording) {
        // Already recording
        return -2;
    }

    if(!cap.isOpened()) {
        // check if we succeeded
        return -1;
    }

    isRecording = true;

    Mat edges;
    while(isRecording) {
        cap >> frame; // Grab a new frame from camera.
        if(!frame.empty()) {
            cvtColor(frame, grey_frame, cv::COLOR_BGR2GRAY);
            comparison = algo->search(img, grey_frame);

            loc = comparison->getLocation();
            offset = comparison->getOffset();
            rectangle(frame, loc, Point( loc.x + offset.x , loc.y + offset.y ), Scalar::all(0), 2, 8, 0 );
            imshow("RC", frame);

            waitKey(100);
        }
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
*/

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