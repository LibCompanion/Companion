#include "Line.h"

Companion::Draw::Line::Line(cv::Point start, cv::Point end, cv::Scalar color, int thickness) {
    this->start = start;
    this->end = end;
    this->color = color;
    this->thickness = thickness;
}

const cv::Point &Companion::Draw::Line::getStart() const {
    return start;
}

void Companion::Draw::Line::setStart(const cv::Point &start) {
    Line::start = start;
}

const cv::Point &Companion::Draw::Line::getEnd() const {
    return end;
}

void Companion::Draw::Line::setEnd(const cv::Point &end) {
    Line::end = end;
}

const cv::Scalar &Companion::Draw::Line::getColor() const {
    return color;
}

void Companion::Draw::Line::setColor(const cv::Scalar &color) {
    Line::color = color;
}

int Companion::Draw::Line::getThickness() const {
    return thickness;
}

void Companion::Draw::Line::setThickness(int thickness) {
    Line::thickness = thickness;
}

void Companion::Draw::Line::draw(cv::Mat image) {
    cv::line(image, start, end, color, thickness);
}

void Companion::Draw::Line::ratio(int cWidth, int cHeight, int nWidth, int nHeight) {
    Util::ratioPosition(start, cWidth, cHeight, nWidth, nHeight);
    Util::ratioPosition(end, cWidth, cHeight, nWidth, nHeight);
}
