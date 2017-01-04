#include "ImageRecognition.h"

Comparison *ImageRecognition::search(cv::Mat search_img, cv::Mat compare_img) {
    try {
        return algo(search_img, compare_img);
    } catch (CompanionError::error_code error) {
        throw error;
    }
}

Comparison *ImageRecognition::search(std::string search_image_path, std::string compare_image_path, int cv_color) {
    Comparison *comparison;

    cv::Mat search_img = cv::imread(search_image_path, cv_color);
    cv::Mat compare_img = cv::imread(compare_image_path, cv_color);

    try {
        comparison = algo(search_img, compare_img);
    } catch (CompanionError::error_code error) {
        throw error;
    }

    // ToDo Check it here for validation error
    search_img.release();
    compare_img.release();

    return comparison;
}