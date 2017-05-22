/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2017 Andreas Sekulski
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

const std::vector<Companion::Draw::Line*> &Companion::Draw::Lines::getLines() const
{
    return this->lines;
}
