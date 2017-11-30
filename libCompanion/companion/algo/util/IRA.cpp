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
	lop.x = NO_OBJECT_DETECTED;
	lop.y = NO_OBJECT_DETECTED;
}

Companion::Algorithm::IRA::~IRA()
{

}

const cv::Rect &Companion::Algorithm::IRA::getLastObjectPosition() const
{
	return lop;
}

void Companion::Algorithm::IRA::setLastObjectPosition(int x, int y, int width, int height)
{
	lop.x = x;
	lop.y = y;
	lop.width = width;
	lop.height = height;
}

void Companion::Algorithm::IRA::setWidth(int width)
{
	lop.width = width;
}

void Companion::Algorithm::IRA::setHeight(int height)
{
	lop.height = height;
}

void Companion::Algorithm::IRA::setX(int x)
{
	lop.x = x;
}

void Companion::Algorithm::IRA::setY(int y)
{
	lop.y = y;
}

void Companion::Algorithm::IRA::clear()
{
	lop.x = NO_OBJECT_DETECTED;
	lop.y = NO_OBJECT_DETECTED;
	lop.width = NO_OBJECT_DETECTED;
	lop.height = NO_OBJECT_DETECTED;
}

bool Companion::Algorithm::IRA::isObjectDetected()
{
	return lop.width > NO_OBJECT_DETECTED && lop.height > NO_OBJECT_DETECTED;
}