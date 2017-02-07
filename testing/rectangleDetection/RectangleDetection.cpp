//
// Created by asekulsk on 11.01.17.
//

#include "RectangleDetection.h"

#include <iostream>

cv::Mat src; cv::Mat src_gray;
int thresh = 100;
int max_thresh = 255;
cv::RNG rng(12345);
cv::Mat dst, detected_edges;

RectangleDetection::RectangleDetection() {}

RectangleDetection::~RectangleDetection() {

}

void variant_one() {
    // http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/bounding_rects_circles/bounding_rects_circles.html

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat threshold_output;

    /// Reduce noise with a kernel 3x3
    blur(src_gray, src_gray, cv::Size(3,3));

    /// Detect edges using Threshold
    Canny(src_gray, threshold_output, 10, thresh);
    //threshold(src_gray, threshold_output, thresh, 255, cv::THRESH_BINARY); // Creates an radius

    /// Find contours
    findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    /// Approximate contours to polygons + get bounding rects
    std::vector<std::vector<cv::Point>> contours_poly(contours.size());
    std::vector<cv::Rect> boundRect;
    cv::Rect bound, compareBound;

    for(int i = 0; i < contours.size(); i++ ) {
        approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true );
        bound = boundingRect(cv::Mat(contours_poly[i]));

        // Don't use small areas from bounding rects
        if(bound.area() > 1000) {
            boundRect.push_back(bound);
        }
    }

    // ToDo := Implement CA-Algo (Cross Area)
    // ToDo := Nice name ;-)
    unsigned long z = 0, x = 0;

    while (z < boundRect.size()) {

        bound = boundRect.at(z);
        compareBound = boundRect.at(x);

        // Check if rectangles overlapping
        // http://answers.opencv.org/question/67091/how-to-find-if-2-rectangles-are-overlapping-each-other/
        if(z != x && (compareBound & bound).area() > 0) {
            // Erase rectangles
            boundRect.erase (boundRect.begin() + z);
            boundRect.erase (boundRect.begin() + x - 1);

            // Union rectangles
            boundRect.push_back((compareBound | bound));

            z = 0; // Begin comparison on the scratch
            x = 0; // Begin comparison on the scratch
        } else {
            x++;
        }

        if(x == boundRect.size()) {
            z++;
            x = 0;
        }
    }

    /// Draw contours
    cv::Scalar color;
    cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3 );
    for(int i = 0; i< contours.size(); i++ ) {
        color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
    }

    cv::Mat result;
    src.copyTo(result);
    /// Draw bonding rects
    for(unsigned long i = 0; i< boundRect.size(); i++ ) {
        color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        bound = boundRect.at(i);
        rectangle(result, bound.tl(), bound.br(), color, 2, 8, 0 );
    }

    /// Show in a window
    cv::namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Contours", drawing);
    imshow("Result", result);
}

void variant_two() {
    // https://pythontips.com/2015/03/11/a-guide-to-finding-books-in-images-using-python-and-opencv/

    cv::Mat temp;
    src_gray.copyTo(temp);

    /// Reduce noise with a kernel 3x3
    blur(temp, temp, cv::Size(3,3));
    cv::imshow("Blur", temp);

    /// Canny detector
    Canny(temp, detected_edges, 10, thresh);
    cv::imshow("Edged", detected_edges);

    cv::Mat closed;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
    morphologyEx(detected_edges, closed, cv::MORPH_CLOSE, kernel);
    cv:imshow("Closed", closed);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Point> contours_poly( contours.size() );
    cv::findContours(closed, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat copy;
    src.copyTo(copy);

    for(size_t i = 0; i< contours.size(); i++ ) {
        double peri = cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], contours_poly, 0.02 * peri, true);

        if(contours_poly.size() == 4) {
            cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
            cv::drawContours(copy, contours, -1, color, 4);
        }
    }

    cv::imshow("Result", copy);
}

void RectangleDetection::thresh_callback(int, void *) {
    variant_one();
    //variant_two();
}

void RectangleDetection::detect(cv::Mat img) {

    src = img;
    //src = cv::imread("/home/asekulsk/Dokumente/Master/handmade-oil-painting-on-canvas-modern-100-Best-Art-scenery-oil-painting-original-directly-from-font.jpg");
    //src = cv::imread("/home/asekulsk/Dokumente/Master/books.jpg");
    //src = cv::imread("/home/asekulsk/Dokumente/Master/gemaelde02g.jpg");

    /// Convert the image to grayscale
    cvtColor(src, src_gray, CV_BGR2GRAY );

    /// Create Window
    char* source_window = "Source";
    cv::namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow(source_window, src);
    /// Create a Trackbar for user to enter threshold
    cv::createTrackbar("Threshold:", source_window, &thresh, max_thresh, thresh_callback );

    /// Show the image
    thresh_callback(0, 0);
}
