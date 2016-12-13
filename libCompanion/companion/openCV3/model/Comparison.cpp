//
// Created by andre on 27.11.2016.
//

#include "Comparison.h"


Comparison::Comparison(double accordance, cv::Point location, cv::Point offset) {
    this->accordance = accordance;
    this->location = location;
    this->offset = offset;
}

Comparison::~Comparison() {}

double Comparison::getAccordance() {
    return accordance;
}

void Comparison::setAccordance(double accordance) {
    Comparison::accordance = accordance;
}


void Comparison::setLocation(cv::Point location) {
    Comparison::location = location;
}

cv::Point Comparison::getLocation() {
    return location;
}

void Comparison::setOffset(cv::Point offset) {
    Comparison::offset = offset;
}

cv::Point Comparison::getOffset() {
    return offset;
}