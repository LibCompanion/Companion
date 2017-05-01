#include "Image.h"

Companion::Input::Image::Image() {
    index = 0;
}

Companion::Input::Image::~Image() {

}

void Companion::Input::Image::addImagePath(std::string path) {
    images.push_back(path);
}

cv::Mat Companion::Input::Image::obtainImage() {

    cv::Mat image;

    if(index < images.size()) {
        // Open image to read
        image = cv::imread(images.at(index));
        // Set next index position to load image.
        index++;
    }

    return image;
}