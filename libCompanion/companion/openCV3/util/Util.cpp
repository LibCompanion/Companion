#include "Util.h"

void Util::resize_image(cv::Mat &img, int size_x, int size_y) {
    cv::resize(img, img, cv::Size(size_x, size_y));
}


bool Util::is_image_loaded(cv::Mat &img) {
    return !img.empty();
}

std::string Util::get_error(CompanionError::error_code code) {

    std::string error = "Unknown Error";

    switch (code) {
        case CompanionError::error_code::descriptor_extractor_not_found:
            error = "Given descriptor not supported";
            break;
        case CompanionError::error_code::descriptor_matcher_not_found:
            error = "Given descriptor matcher not supported";
            break;
        case CompanionError::error_code::dimension_error:
            error = "Dimensions not equal";
            break;
        case CompanionError::error_code::feature_detector_not_found:
            error = "Given feature not supported";
            break;
        case CompanionError::error_code::image_not_found:
            error = "Could not open or find image";
            break;
        case CompanionError::error_code::template_dimension_error:
            error = "Template size must be smaller or equal than image";
            break;
        case CompanionError::error_code::wrong_model_type:
            error = "This model type class is not supported for this image recognition algorithm";
            break;
    }

    return error;
}