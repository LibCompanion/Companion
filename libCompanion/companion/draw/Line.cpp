#include "Line.h"

Line::Line(cv::Point start, cv::Point end, cv::Scalar color, int thickness) {
    this->start = start;
    this->end = end;
    this->color = color;
    this->thickness = thickness;
}

const cv::Point &Line::getStart() const {
    return start;
}

void Line::setStart(const cv::Point &start) {
    Line::start = start;
}

const cv::Point &Line::getEnd() const {
    return end;
}

void Line::setEnd(const cv::Point &end) {
    Line::end = end;
}

const cv::Scalar &Line::getColor() const {
    return color;
}

void Line::setColor(const cv::Scalar &color) {
    Line::color = color;
}

int Line::getThickness() const {
    return thickness;
}

void Line::setThickness(int thickness) {
    Line::thickness = thickness;
}

void Line::draw(cv::Mat image) {
    cv::line(image, start, end, color, thickness);
}

void Line::ratio(int cWidth, int cHeight, int nWidth, int nHeight) {
    Util::ratioPosition(start, cWidth, cHeight, nWidth, nHeight);
    Util::ratioPosition(end, cWidth, cHeight, nWidth, nHeight);
}
