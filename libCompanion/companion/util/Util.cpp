#include "Util.h"

bool Util::isImageLoaded(cv::Mat &img) {
    return !img.empty();
}

void Util::resizeImage(cv::Mat &img, int size_x, int size_y) {
    cv::resize(img, img, cv::Size(size_x, size_y));
}

void Util::ratioPosition(cv::Point &point, int cWidth, int cHeight, int nWidth, int nHeight) {
    point.x = (int)(((float)point.x / (float)cWidth) * nWidth);
    point.y = (int)(((float)point.y / (float)cHeight) * nHeight);
}