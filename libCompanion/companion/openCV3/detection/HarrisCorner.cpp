//
// Created by asekulsk on 01.12.16.
//

#include "HarrisCorner.h"


HarrisCorner::HarrisCorner() {

}

HarrisCorner::~HarrisCorner() {

}

void HarrisCorner::doNothing(Mat image) {

    int thresh = 220;

    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros(image.size(), CV_32FC1);

    /// Detector parameters
    int blockSize = 6;
    int apertureSize = 3;
    double k = 0.04;

    /// Detecting corners
    cornerHarris(image, dst, blockSize, apertureSize, k);

    /// Normalizing
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);

    /// Drawing a circle around corners

    for (int j = 0; j < dst_norm.rows; j++)
    {
        for (int i = 0; i < dst_norm.cols; i++)
        {
            if ((int)dst_norm.at<float>(j, i) > thresh)
            {
                circle(dst_norm_scaled, Point(i, j), 5, Scalar(224, 245, 255), 2, 8, 0);
            }
        }
    }

    /// Showing the result
    imshow("Corners detected", dst_norm_scaled);
}