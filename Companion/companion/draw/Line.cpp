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

const cv::Point& Companion::Draw::Line::Start() const
{
	return this->start;
}

void Companion::Draw::Line::Start(const cv::Point& start)
{
	this->start = start;
}

const cv::Point& Companion::Draw::Line::End() const
{
	return this->end;
}

void Companion::Draw::Line::End(const cv::Point& end)
{
	this->end = end;
}

const cv::Scalar& Companion::Draw::Line::Color() const
{
	return this->color;
}

void Companion::Draw::Line::Color(const cv::Scalar& color)
{
	this->color = color;
}

int Companion::Draw::Line::Thickness() const
{
	return this->thickness;
}

void Companion::Draw::Line::Thickness(int thickness)
{
	this->thickness = thickness;
}

void Companion::Draw::Line::Draw(cv::Mat image)
{
	cv::line(image, this->start, this->end, this->color, this->thickness);
}

void Companion::Draw::Line::Ratio(int cWidth, int cHeight, int nWidth, int nHeight)
{
	Util::RatioPosition(this->start, cWidth, cHeight, nWidth, nHeight);
	Util::RatioPosition(this->end, cWidth, cHeight, nWidth, nHeight);
}

cv::Rect Companion::Draw::Line::CutArea()
{
	return cv::Rect(this->start.x, this->start.y, this->start.x + this->end.x, this->start.y + this->end.y);
}

int Companion::Draw::Line::OriginX() {
	return this->start.x;
}

int Companion::Draw::Line::OriginY() {
	return this->start.y;
}

void Companion::Draw::Line::MoveOrigin(int x, int y) {
	this->start.x = this->start.x + x;
	this->start.y = this->start.y + y;
	this->end.x = this->end.x + x;
	this->end.y = this->end.y + y;
}
