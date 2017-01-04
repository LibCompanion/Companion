//
// Created by asekulsk on 04.01.17.
//

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