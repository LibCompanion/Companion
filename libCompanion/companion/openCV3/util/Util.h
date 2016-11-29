#ifndef COMPANION_UTIL_H
#define COMPANION_UTIL_H

#include <iostream>
#include <opencv2/core/core.hpp>

#include "companion/openCV3/util/CompanionError.h"

using namespace std;
using namespace cv;

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
        static bool is_image_loaded(Mat &img);

        /**
         * @brief Gets from given error code an corresponding error message.
         *
         * @param e_code The error code to get corresponding error message.
         *
         * @return String message from given error.
         */
        static string get_error(CompanionError::error_code code);

};

#endif //COMPANION_UTIL_H