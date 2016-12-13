#include "SimpleCompareImage.h"

SimpleCompareImage::SimpleCompareImage() {

}

SimpleCompareImage::~SimpleCompareImage() {

}

Comparison* SimpleCompareImage::algo(cv::Mat search_img, cv::Mat compare_img) {

    int pixels;
    int equalPixels;
    double accordance;
    cv::Mat result;

    // Check if image is loaded
    if (!Util::is_image_loaded(search_img) || !Util::is_image_loaded(compare_img)) {
        throw CompanionError::error_code::image_not_found;
    }

    // Check if dimension from img is equal to compare_image
    if (search_img.cols != compare_img.cols || search_img.rows != compare_img.rows || search_img.dims != compare_img.dims) {
        throw CompanionError::error_code::dimension_error;
    }

    // Calculate pixel size from image.
    pixels = search_img.cols * search_img.rows;

    // Compare images for equality
    compare(search_img, compare_img, result, cv::CMP_EQ);

    equalPixels = countNonZero(result);
    accordance = (double) equalPixels / (double) pixels;

    // Image location is complete image in simple compare image.
    cv::Point matchLoc = cv::Point(0, 0);
    cv::Point matchOffset = cv::Point(compare_img.cols, compare_img.rows);

    return new Comparison(accordance, matchLoc, matchOffset);
}