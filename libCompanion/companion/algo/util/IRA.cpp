#include "IRA.h"

Companion::Algorithm::IRA::IRA() {
    lop.x = NO_OBJECT_DETECTED;
    lop.y = NO_OBJECT_DETECTED;
}

Companion::Algorithm::IRA::~IRA() {

}

const cv::Rect &Companion::Algorithm::IRA::getLastObjectPosition() const {
    return lop;
}

void Companion::Algorithm::IRA::setLastObjectPosition(int x, int y, int width, int height) {
    lop.x = x;
    lop.y = y;
    lop.width = width;
    lop.height = height;
}

void Companion::Algorithm::IRA::setWidth(int width) {
    lop.width = width;
}

void Companion::Algorithm::IRA::setHeight(int height) {
    lop.height = height;
}

void Companion::Algorithm::IRA::setX(int x) {
    lop.x = x;
}

void Companion::Algorithm::IRA::setY(int y) {
    lop.y = y;
}

void Companion::Algorithm::IRA::clear() {
    lop.x = NO_OBJECT_DETECTED;
    lop.y = NO_OBJECT_DETECTED;
    lop.width = NO_OBJECT_DETECTED;
    lop.height = NO_OBJECT_DETECTED;
}

bool Companion::Algorithm::IRA::isObjectDetected() {
    // ToDo := Currently 10000 because of an IFIS video bug to small areas for IRA...
    return lop.area() > 10000 && lop.width > NO_OBJECT_DETECTED && lop.height > NO_OBJECT_DETECTED;
}