#include "ImageRecognition.h"

Comparison* ImageRecognition::search(Mat search_img, Mat compare_img) {
    try {
        return algo(search_img, compare_img);
    } catch (CompanionError::error_code error) {
        throw error;
    }
}

Comparison* ImageRecognition::search(string search_image_path, string compare_image_path, int cv_color) {
    Comparison *comparison;

    Mat search_img = imread(search_image_path, cv_color);
    Mat compare_img = imread(compare_image_path, cv_color);

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