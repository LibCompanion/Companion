/*
 * This program is an object recognition framework written with OpenCV.
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

#include "IRA.h"

Companion::Algorithm::Recognition::Matching::IRA::IRA()
{
	this->lop.x = NO_OBJECT_RECOGNIZED;
	this->lop.y = NO_OBJECT_RECOGNIZED;
}

const cv::Rect& Companion::Algorithm::Recognition::Matching::IRA::LastObjectPosition() const
{
	return this->lop;
}

void Companion::Algorithm::Recognition::Matching::IRA::LastObjectPosition(int x, int y, int width, int height)
{
	this->lop.x = x;
	this->lop.y = y;
	this->lop.width = width;
	this->lop.height = height;
}

void Companion::Algorithm::Recognition::Matching::IRA::Width(int width)
{
	this->lop.width = width;
}

void Companion::Algorithm::Recognition::Matching::IRA::Height(int height)
{
	this->lop.height = height;
}

void Companion::Algorithm::Recognition::Matching::IRA::X(int x)
{
	this->lop.x = x;
}

void Companion::Algorithm::Recognition::Matching::IRA::Y(int y)
{
	this->lop.y = y;
}

void Companion::Algorithm::Recognition::Matching::IRA::Clear()
{
	this->lop.x = NO_OBJECT_RECOGNIZED;
	this->lop.y = NO_OBJECT_RECOGNIZED;
	this->lop.width = NO_OBJECT_RECOGNIZED;
	this->lop.height = NO_OBJECT_RECOGNIZED;
}

bool Companion::Algorithm::Recognition::Matching::IRA::IsObjectRecognized()
{
	return (this->lop.width > NO_OBJECT_RECOGNIZED) && (this->lop.height > NO_OBJECT_RECOGNIZED);
}
