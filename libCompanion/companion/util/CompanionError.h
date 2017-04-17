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
    enum class errorCode {
        image_not_found, ///< If image not found error.
        dimension_error, ///< If dimensions from given images inequal.
        template_dimension_error, ///< If dimensions from template is wrong.
        feature_detector_not_found, ///< If given feature detector not supported.
        descriptor_extractor_not_found, ///< If given descriptor extractor not supported.
        descriptor_matcher_not_found, ///< If given descriptor matcher not supported.
        wrong_model_type, ///< If given model class type is not supported for an image recognition search.
        invalid_companion_config, ///< If given configuration is invalid.
        video_src_not_set, ///< If given video source is not set.
        invalid_video_src, ///< If given video source is invalid like an error path.
        no_image_processing_algo_set, ///< If no image processing algo is used.
        no_handler_set ///< If no callback handler is set.
    };

    /**
     * @brief Gets from given error code an corresponding error message.
     * @param code The error code to get corresponding error message.
     * @return String message from given error.
     */
    inline std::string getError(errorCode code) {

        std::string error = "Unknown Error";

        switch (code) {
            case errorCode::descriptor_extractor_not_found:
                error = "Given descriptor not supported.";
                break;
            case errorCode::descriptor_matcher_not_found:
                error = "Given descriptor matcher not supported.";
                break;
            case errorCode::dimension_error:
                error = "Dimensions not equal.";
                break;
            case errorCode::feature_detector_not_found:
                error = "Given feature not supported.";
                break;
            case errorCode::image_not_found:
                error = "Could not open or find image.";
                break;
            case errorCode::template_dimension_error:
                error = "Template size must be smaller or equal than image.";
                break;
            case errorCode::wrong_model_type:
                error = "This model type class is not supported for this image recognition algorithm.";
                break;
            case errorCode::invalid_companion_config:
                error = "Invalid companion configuration.";
                break;
            case errorCode::video_src_not_set:
                error = "Video source is not set.";
                break;
            case errorCode::invalid_video_src:
                error = "Video src is not obtainable.";
                break;
            case errorCode::no_image_processing_algo_set:
                error = "No image processing algorithm is used.";
                break;
            case errorCode::no_handler_set:
                error = "No callback handler set";
                break;
        }

        return error;
    }
}

#endif //COMPANION_ERROR_H