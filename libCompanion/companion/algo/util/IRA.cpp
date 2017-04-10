#include "IRA.h"

IRA::IRA() {
    lop.x = NO_OBJECT_DETECTED;
    lop.y = NO_OBJECT_DETECTED;
}

IRA::~IRA() {

}

const cv::Rect &IRA::getLastObjectPosition() const {
    return lop;
}

void IRA::setLastObjectPosition(int x, int y, int width, int height) {
    lop.x = x;
    lop.y = y;
    lop.width = width;
    lop.height = height;
}

void IRA::setWidth(int width) {
    lop.width = width;
}

void IRA::setHeight(int height) {
    lop.height = height;
}

void IRA::setX(int x) {
    lop.x = x;
}

void IRA::setY(int y) {
    lop.y = y;
}

void IRA::clear() {
    lop.x = NO_OBJECT_DETECTED;
    lop.y = NO_OBJECT_DETECTED;
    lop.width = NO_OBJECT_DETECTED;
    lop.height = NO_OBJECT_DETECTED;
}

bool IRA::isObjectDetected() {
    // ToDo := Currently 10000 because of an IFIS video bug to small areas for IRA...
    return lop.area() > 10000 && lop.width > NO_OBJECT_DETECTED && lop.height > NO_OBJECT_DETECTED;
}