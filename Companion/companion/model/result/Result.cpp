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

#include "Result.h"

Companion::Model::Result::Result::Result(int scoring, PTR_DRAW drawable)
{
    this->scoring = scoring;
    this->drawable = drawable;
}

int Companion::Model::Result::Result::Scoring() const
{
    return this->scoring;
}

PTR_DRAW Companion::Model::Result::Result::Drawable() const
{
	return this->drawable;
}
