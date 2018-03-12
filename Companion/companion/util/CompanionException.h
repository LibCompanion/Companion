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

#ifndef COMPANION_COMPANIONEXCEPTION_H
#define COMPANION_COMPANIONEXCEPTION_H

#include <vector>
#include <stdexcept>
#include <companion/util/exportapi/ExportAPIDefinitions.h>
#include <companion/util/CompanionError.h>

namespace Companion { namespace Error {

    /**
     * Companion exception class implementation for multiple error messages obtained from parallelized methods.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS CompanionException : public std::exception {

    public:

        /**
         * Constructor to store all error codes.
         * @param errors Error codes to store exceptions.
         */
        CompanionException(std::vector<Companion::Error::Code> errors);

        /**
         * Check if exception has errors to execute.
         * @return True if has next error codes otherwise false.
         */
        bool hasNext();

        /**
         * Obtain next error code.
         * @return Next error code.
         */
        Companion::Error::Code next();

    private:

        /**
         * List of all collected error codes.
         */
        std::vector<Companion::Error::Code> errors;

        /**
         * Current index for next error code.
         */
        size_t index;
    };
}}

#endif //COMPANION_COMPANIONEXCEPTION_H
