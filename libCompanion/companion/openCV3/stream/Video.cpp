//
// Created by andre on 26.11.2016.
//

#include "Video.h"

#include <thread>

Video::Video() {

}

Video::~Video() {

}

int Video::realtime(ImageRecognition *algo, Mat img) {

    Comparison *comparison;
    Point loc;
    Point offset;

    HarrisCorner corner;

    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened()) {
        // check if we succeeded
        return -1;
    }

    Mat edges;
    while(true)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        if(!frame.empty()) {

            cvtColor(frame, edges, CV_BGR2GRAY);

            corner.doNothing(edges);

            //comparison = algo->search(frame, img);
            //loc = comparison->getLocation();
            //offset = comparison->getOffset();
            //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
            //Canny(edges, edges, 0, 30, 3);

            //rectangle(edges, loc, Point( loc.x + offset.x , loc.y + offset.y ), Scalar::all(0), 2, 8, 0 );
            imshow("RC", frame);

            waitKey(50);
        }
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}