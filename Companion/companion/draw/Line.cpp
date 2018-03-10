/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2018 Andreas Sekulski, Dimitri Kotlovsky
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
    return this->start;
}

void Companion::Draw::Line::setStart(const cv::Point &start)
{
    this->start = start;
}

const cv::Point &Companion::Draw::Line::getEnd() const
{
    return this->end;
}

void Companion::Draw::Line::setEnd(const cv::Point &end)
{
    this->end = end;
}

const cv::Scalar &Companion::Draw::Line::getColor() const
{
    return this->color;
}

void Companion::Draw::Line::setColor(const cv::Scalar &color)
{
    this->color = color;
}

int Companion::Draw::Line::getThickness() const
{
    return this->thickness;
}

void Companion::Draw::Line::setThickness(int thickness)
{
    this->thickness = thickness;
}

void Companion::Draw::Line::draw(cv::Mat image)
{
    cv::line(image, this->start, this->end, this->color, this->thickness);
}

void Companion::Draw::Line::ratio(int cWidth, int cHeight, int nWidth, int nHeight)
{
    Util::ratioPosition(this->start, cWidth, cHeight, nWidth, nHeight);
    Util::ratioPosition(this->end, cWidth, cHeight, nWidth, nHeight);
}

cv::Rect Companion::Draw::Line::getCutArea()
{
    return cv::Rect(this->start.x, this->start.y, this->start.x + this->end.x, this->start.y + this->end.y);
}

int Companion::Draw::Line::getOriginX() {
    return this->start.x;
}

int Companion::Draw::Line::getOriginY() {
    return this->start.y;
}

void Companion::Draw::Line::moveOrigin(int x, int y) {
    this->start.x = this->start.x + x;
    this->start.y = this->start.y + y;
    this->end.x = this->end.x + x;
    this->end.y = this->end.y + y;
}
