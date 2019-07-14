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

#include "RecognitionResult.h"

Companion::Model::Result::RecognitionResult::RecognitionResult(int scoring, int id, PTR_DRAW drawable) : Result(scoring, drawable)
{
	this->id = id;
}

int Companion::Model::Result::RecognitionResult::Id() const
{
	return this->id;
}

std::string Companion::Model::Result::RecognitionResult::Description() const
{
	return std::to_string(this->id);
}

Companion::Model::Result::ResultType Companion::Model::Result::RecognitionResult::Type() const
{
	return ResultType::RECOGNITION;
}
