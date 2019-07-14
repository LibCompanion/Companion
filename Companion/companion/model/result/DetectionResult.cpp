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

#include "DetectionResult.h"

Companion::Model::Result::DetectionResult::DetectionResult(int scoring, std::string objectType, PTR_DRAW drawable) : Result(scoring, drawable)
{
    this->objectType = objectType;
}

std::string Companion::Model::Result::DetectionResult::ObjectType() const
{
    return this->objectType;
}

std::string Companion::Model::Result::DetectionResult::Description() const
{
    return this->objectType;
}

Companion::Model::Result::ResultType Companion::Model::Result::DetectionResult::Type() const
{
    return ResultType::DETECTION;
}
