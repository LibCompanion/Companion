/*
* This program is an image recognition library written with OpenCV.
* Copyright (C) 2016-2018 Andreas Sekulski
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

#include "CompanionException.h"

Companion::Error::CompanionException::CompanionException(std::vector<Companion::Error::Code> errors)
{
    this->errors = errors;
    this->index = 0;
}

bool Companion::Error::CompanionException::hasNext()
{
    return index < this->errors.size();
}

Companion::Error::Code Companion::Error::CompanionException::next()
{
    Companion::Error::Code code = this->errors.at(index);
    index++;
    return code;
}
