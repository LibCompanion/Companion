//
// Created by asekulsk on 08.12.16.
//

#ifndef COMPANION_OBJECTTRACKING_H
#define COMPANION_OBJECTTRACKING_H

#include <sstream>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <cv.hpp>

using namespace std;
using namespace cv;

class ObjectTracking {

public:

    ObjectTracking();

    void start();

};

#endif //COMPANION_OBJECTTRACKING_H
