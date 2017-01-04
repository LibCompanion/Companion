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

#ifndef COMPANION_ERROR_H
#define COMPANION_ERROR_H

/**
 * Enumeration namespace for errors.
 * @author Andreas Sekulski
 */
namespace CompanionError {
    /// Enumeration class for error codes
    enum class error_code {
        image_not_found, ///< If image not found error.
        dimension_error, ///< If dimensions from given images inequal.
        template_dimension_error, ///< If dimensions from template is wrong.
        feature_detector_not_found, ///< If given feature detector not supported.
        descriptor_extractor_not_found,  ///< If given descriptor extractor not supported.
        descriptor_matcher_not_found ///< If given descriptor matcher not supported.
    };
}

#endif //COMPANION_ERROR_H