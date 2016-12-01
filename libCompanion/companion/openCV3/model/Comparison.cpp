//
// Created by andre on 27.11.2016.
//

#include "Comparison.h"


Comparison::Comparison(double accordance, Point location, Point offset) {
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


void Comparison::setLocation(Point location) {
    Comparison::location = location;
}

Point Comparison::getLocation() {
    return location;
}

void Comparison::setOffset(Point offset) {
    Comparison::offset = offset;
}

Point Comparison::getOffset() {
    return offset;
}