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


    for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
    {
        double a=contourArea( contours[i],false);  //  Find the area of contour
        if(a>largest_area){
            largest_area=a;
            largest_contour_index=i;                //Store the index of largest contour
            bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
        }
    }

    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) ); // RND COLOR AWESOME
    drawContours( drawing, contours,largest_contour_index, color, CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
    rectangle(src, bounding_rect,  Scalar(255,255,255),1, 8,0);


/*
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
    }
*/

    namedWindow("Source", WINDOW_AUTOSIZE );
    imshow("Source", src );

    namedWindow( "Contours", WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
}

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
            GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
            Canny(edges, edges, 0, 30, 3);

            imshow("RC", edges);

            src = frame;

            if (src.empty()) {
                cerr << "No image supplied ..." << endl;
                return -1;
            }

            src_gray = frame;

          //  cvtColor( src, src_gray, COLOR_BGR2GRAY );
            blur( src_gray, src_gray, Size(3,3) );
            const char* source_window = "Source";
            createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
            thresh_callback( 0, 0 );

            //comparison = algo->search(frame, img);
            //loc = comparison->getLocation();
            //offset = comparison->getOffset();
            //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
            //Canny(edges, edges, 0, 30, 3);
            //corner.doConturs(edges);

            //rectangle(edges, loc, Point( loc.x + offset.x , loc.y + offset.y ), Scalar::all(0), 2, 8, 0 );
            //imshow("RC", frame);

            waitKey(100);
        }
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}