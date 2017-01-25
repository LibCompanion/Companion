//
// Created by asekulsk on 11.01.17.
//

#ifndef COMPANION_RECTANGLEDETECTION_H
#define COMPANION_RECTANGLEDETECTION_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stack>

class RectangleDetection {

public:
    RectangleDetection();

    virtual ~RectangleDetection();

    static void thresh_callback(int, void* );

    void detect(cv::Mat img);
};


#endif //COMPANION_RECTANGLEDETECTION_H
