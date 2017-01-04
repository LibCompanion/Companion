// This is currently temporarily implemented.
// ToDo := Do i need it?

#ifndef COMPANION_HARRISCORNER_H
#define COMPANION_HARRISCORNER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

// ToDo := Do i need it?!
class HarrisCorner {

public:
    HarrisCorner();

    ~HarrisCorner();

    void doNothing(cv::Mat image);

    void doConturs(cv::Mat image);
};


#endif //COMPANION_HARRISCORNER_H
