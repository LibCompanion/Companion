//
// Created by asekulsk on 01.12.16.
//

#ifndef COMPANION_HARRISCORNER_H
#define COMPANION_HARRISCORNER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

class HarrisCorner {

    public:
        HarrisCorner();

        ~HarrisCorner();

        void doNothing(cv::Mat image);

        void doConturs(cv::Mat image);
};


#endif //COMPANION_HARRISCORNER_H
