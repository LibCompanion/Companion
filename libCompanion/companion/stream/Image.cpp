#include "Image.h"

Image::Image() {
    index = 0;
}

Image::~Image() {

}

void Image::addImagePath(std::string path) {
    images.push_back(path);
}

cv::Mat Image::obtainImage() {

    cv::Mat image;

    if(index < images.size()) {
        // Open image to read
        image = cv::imread(images.at(index));
        // Set next index position to load image.
        index++;
    }

    return image;
}