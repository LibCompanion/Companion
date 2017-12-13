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

#include "Line.h"

Companion::Draw::Line::Line(cv::Point start, cv::Point end, cv::Scalar color, int thickness)
{
    this->start = start;
    this->end = end;
    this->color = color;
    this->thickness = thickness;
}

const cv::Point &Companion::Draw::Line::getStart() const
{
    return start;
}

void Companion::Draw::Line::setStart(const cv::Point &start)
{
    Line::start = start;
}

const cv::Point &Companion::Draw::Line::getEnd() const
{
    return end;
}

void Companion::Draw::Line::setEnd(const cv::Point &end)
{
    Line::end = end;
}

const cv::Scalar &Companion::Draw::Line::getColor() const
{
    return color;
}

void Companion::Draw::Line::setColor(const cv::Scalar &color)
{
    Line::color = color;
}

int Companion::Draw::Line::getThickness() const
{
    return thickness;
}

void Companion::Draw::Line::setThickness(int thickness)
{
    Line::thickness = thickness;
}

void Companion::Draw::Line::draw(cv::Mat image)
{
    cv::line(image, start, end, color, thickness);
}

void Companion::Draw::Line::ratio(int cWidth, int cHeight, int nWidth, int nHeight)
{
    Util::ratioPosition(start, cWidth, cHeight, nWidth, nHeight);
    Util::ratioPosition(end, cWidth, cHeight, nWidth, nHeight);
}

cv::Rect Companion::Draw::Line::cutArea()
{
    return cv::Rect(start.x, start.y, start.x + end.x, start.y + end.y);
}

int Companion::Draw::Line::getGroundZeroX() {
    return start.x;
}

int Companion::Draw::Line::getGroundZeroY() {
    return start.y;
}

void Companion::Draw::Line::moveGroundZero(int x, int y) {
    start.x = start.x + x;
    start.y = start.y + y;

    end.x = end.x + x;
    end.y = end.y + y;
}