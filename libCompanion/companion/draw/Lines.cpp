#include "Lines.h"

Lines::Lines() {

}

Lines::~Lines() {

}

void Lines::addLine(Line *line) {
    this->lines.push_back(line);
}

void Lines::draw(cv::Mat image) {
    for (auto &line : lines) {
        line->draw(image);
    }
}

void Lines::ratio(int cWidth, int cHeight, int nWidth, int nHeight) {
    for (auto &line : lines) {
        line->ratio(cWidth, cHeight, nWidth, nHeight);
    }
};