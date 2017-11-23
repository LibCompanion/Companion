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

#include "IRA.h"

Companion::Algorithm::IRA::IRA()
{
	this->lop.x = NO_OBJECT_DETECTED;
    this->lop.y = NO_OBJECT_DETECTED;
}

Companion::Algorithm::IRA::~IRA()
{

}

const cv::Rect &Companion::Algorithm::IRA::getLastObjectPosition() const
{
	return this->lop;
}

void Companion::Algorithm::IRA::setLastObjectPosition(int x, int y, int width, int height)
{
    this->lop.x = x;
    this->lop.y = y;
    this->lop.width = width;
    this->lop.height = height;
}

void Companion::Algorithm::IRA::setWidth(int width)
{
    this->lop.width = width;
}

void Companion::Algorithm::IRA::setHeight(int height)
{
    this->lop.height = height;
}

void Companion::Algorithm::IRA::setX(int x)
{
    this->lop.x = x;
}

void Companion::Algorithm::IRA::setY(int y)
{
    this->lop.y = y;
}

void Companion::Algorithm::IRA::clear()
{
    this->lop.x = NO_OBJECT_DETECTED;
    this->lop.y = NO_OBJECT_DETECTED;
    this->lop.width = NO_OBJECT_DETECTED;
    this->lop.height = NO_OBJECT_DETECTED;
}

bool Companion::Algorithm::IRA::isObjectDetected()
{
	// ToDo := Currently 10000 because of an IFIS video bug to small areas for IRA...
	return (this->lop.area() > 10000) && (this->lop.width > NO_OBJECT_DETECTED) && (this->lop.height > NO_OBJECT_DETECTED);
}