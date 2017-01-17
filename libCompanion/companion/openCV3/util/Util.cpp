#include "Util.h"

bool Util::isImageLoaded(cv::Mat &img) {
    return !img.empty();
}

void Util::resizeImage(cv::Mat &img, int size_x, int size_y) {
    cv::resize(img, img, cv::Size(size_x, size_y));
}