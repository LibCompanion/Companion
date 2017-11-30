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

#include "Frame.h"

Companion::Draw::Frame::Frame(cv::Point topLeft,
	cv::Point topRight,
	cv::Point bottomLeft,
	cv::Point bottomRight,
	cv::Scalar color,
	int thickness)
{
	this->topLeft = topLeft;
	this->topRight = topRight;
	this->bottomLeft = bottomLeft;
	this->bottomRight = bottomRight;
	this->color = color;
	this->thickness = thickness;
}

Companion::Draw::Frame::~Frame() {}

void Companion::Draw::Frame::draw(cv::Mat image)
{
	Line* top = new Companion::Draw::Line(topLeft, topRight, color, thickness);
	Line* left = new Companion::Draw::Line(topLeft, bottomLeft, color, thickness);
	Line* right = new Companion::Draw::Line(topRight, bottomRight, color, thickness);
	Line* bikini_bottom = new Companion::Draw::Line(bottomLeft, bottomRight, color, thickness); // Easter egg :-)

	// Draw it
	top->draw(image);
	left->draw(image);
	right->draw(image);
	bikini_bottom->draw(image);

	// Cleanup after draw
	delete top;
	delete left;
	delete right;
	delete bikini_bottom;
}

void Companion::Draw::Frame::ratio(int cWidth, int cHeight, int nWidth, int nHeight)
{
	Util::ratioPosition(topLeft, cWidth, cHeight, nWidth, nHeight);
	Util::ratioPosition(topRight, cWidth, cHeight, nWidth, nHeight);
	Util::ratioPosition(bottomLeft, cWidth, cHeight, nWidth, nHeight);
	Util::ratioPosition(bottomRight, cWidth, cHeight, nWidth, nHeight);
}

const cv::Point &Companion::Draw::Frame::getTopLeft() const
{
	return topLeft;
}

const cv::Point &Companion::Draw::Frame::getTopRight() const
{
	return topRight;
}

const cv::Point &Companion::Draw::Frame::getBottomLeft() const
{
	return bottomLeft;
}

const cv::Point &Companion::Draw::Frame::getBottomRight() const
{
	return bottomRight;
}

const cv::Scalar &Companion::Draw::Frame::getColor() const
{
	return color;
}

cv::Rect Companion::Draw::Frame::cutArea()
{
	return cv::Rect(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
}

int Companion::Draw::Frame::getThickness() const
{
	return thickness;
}

void Companion::Draw::Frame::setTopLeft(const cv::Point &topLeft)
{
	Frame::topLeft = topLeft;
}

void Companion::Draw::Frame::setTopRight(const cv::Point &topRight)
{
	Frame::topRight = topRight;
}

void Companion::Draw::Frame::setBottomLeft(const cv::Point &bottomLeft)
{
	Frame::bottomLeft = bottomLeft;
}

void Companion::Draw::Frame::setBottomRight(const cv::Point &bottomRight)
{
	Frame::bottomRight = bottomRight;
}

void Companion::Draw::Frame::setColor(const cv::Scalar &color)
{
	Frame::color = color;
}

void Companion::Draw::Frame::setThickness(int thickness)
{
	Frame::thickness = thickness;
};