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

#include "Result.h"

Companion::Model::Result::Result(int scoring, int id, Companion::Draw::Drawable *model)
{
	this->scoring = scoring;
	this->id = id;
	this->model = model;
}

Companion::Model::Result::~Result() {}

int Companion::Model::Result::getScoring() const
{
	return this->scoring;
}

Companion::Draw::Drawable *Companion::Model::Result::getModel() const
{
	return this->model;
}

int Companion::Model::Result::getId() const
{
	return this->id;
}
