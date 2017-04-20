#include "Lines.h"

Companion::Draw::Lines::Lines() {

}

Companion::Draw::Lines::~Lines() {

}

void Companion::Draw::Lines::addLine(Line *line) {
    this->lines.push_back(line);
}

void Companion::Draw::Lines::draw(cv::Mat image) {
    for (auto &line : lines) {
        line->draw(image);
    }
}

void Companion::Draw::Lines::ratio(int cWidth, int cHeight, int nWidth, int nHeight) {
    for (auto &line : lines) {
        line->ratio(cWidth, cHeight, nWidth, nHeight);
    }
};