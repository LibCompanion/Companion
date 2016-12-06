//
// Created by andre on 26.11.2016.
//

#include "Video.h"

#include <thread>


Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, void* )
{
    int largest_area=0;
    int largest_contour_index=0;
    Rect bounding_rect;

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Canny(src_gray, canny_output, thresh, thresh*2, 3 );

    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );

    /*
    for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
    {
        double a=contourArea( contours[i],false);  //  Find the area of contour
        if(a>largest_area){
            largest_area=a;
            largest_contour_index=i;                //Store the index of largest contour
            bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
        }
    }
*/
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) ); // RND COLOR AWESOME
    drawContours( drawing, contours,largest_contour_index, color, CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
    rectangle(src, bounding_rect,  Scalar(255,255,255),1, 8,0);

    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
        rectangle(src, boundingRect(contours[i]),  Scalar(0,0,0),1, 8,0);
    }


    namedWindow("Source", WINDOW_AUTOSIZE );
    imshow("Source", src );

    namedWindow( "Contours", WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
}

Video::Video() {
    isRecording = false;
}

Video::~Video() {
    if(isRecording) {
        stopRealtime();
    }
}

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
/*
            loc = comparison->getLocation();
            offset = comparison->getOffset();
            rectangle(frame, loc, Point( loc.x + offset.x , loc.y + offset.y ), Scalar::all(0), 2, 8, 0 );
            imshow("RC", frame);
*/
            waitKey(100);
        }
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

void Video::stopRealtime() {
    isRecording = false;
}