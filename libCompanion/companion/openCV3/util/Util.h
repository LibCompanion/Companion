#ifndef COMPANION_UTIL_H
#define COMPANION_UTIL_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "companion/openCV3/util/CompanionError.h"

/**
 * Utility class for opencv.
 *
 * @author Andreas Sekulksi
 */
class Util {

public:

    /**
     * Checks if given image is loaded.
     *
     * @param img Image to check if it's not null.
     *
     * @return <b>TRUE</b> if image is loaded otherwise <b>False<b>
     */
    static bool is_image_loaded(cv::Mat &img);

    /**
     * @brief Gets from given error code an corresponding error message.
     *
     * @param e_code The error code to get corresponding error message.
     *
     * @return String message from given error.
     */
    static std::string get_error(CompanionError::error_code code);

    static void resize_image(cv::Mat &img, int size_x, int size_y);

};

#endif //COMPANION_UTIL_H