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

#ifndef COMPANION_DEFINITIONS_H
#define COMPANION_DEFINITIONS_H

#include <companion/model/result/Result.h>

namespace Companion
{
    /**
     * Callback result definition.
     */
    #define CALLBACK_RESULT std::vector<Companion::Model::Result::Result*>

     /**
      * Default success callback function declaration to obtain results from algorithms.
      */
    #define SUCCESS_CALLBACK void(CALLBACK_RESULT, cv::Mat)

      /**
       * Default error callback function declaration to obtain error results from companion.
       */
    #define ERROR_CALLBACK void(Companion::Error::Code)
}

#endif //COMPANION_DEFINITIONS_H
