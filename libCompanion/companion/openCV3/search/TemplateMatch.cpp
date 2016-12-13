#include "TemplateMatch.h"

TemplateMatch::TemplateMatch() {
    this->match_method = CV_TM_SQDIFF_NORMED;
}

TemplateMatch::TemplateMatch(int match_method) {
    this->match_method = match_method;
}

void TemplateMatch::set_match_method(int match_method) {
    TemplateMatch::match_method = match_method;
}

Comparison* TemplateMatch::algo(cv::Mat search_img, cv::Mat compare_img) {

    cv::Mat result;

    // Check if image is loaded
    if (!Util::is_image_loaded(search_img) || !Util::is_image_loaded(compare_img)) {
        throw CompanionError::error_code::image_not_found;
    }

    // If template is greater than image size
    if (search_img.cols < compare_img.cols || search_img.rows < compare_img.rows) {
        throw CompanionError::error_code::template_dimension_error;
    }

    // Create the result matrix
    int result_cols = search_img.cols - compare_img.cols + 1;
    int result_rows = search_img.rows - compare_img.rows + 1;

    result.create(result_rows, result_cols, CV_32FC1);

    // Methods : CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED
    matchTemplate(search_img, compare_img, result, match_method);
    threshold(result, result, 0.1, 1., CV_THRESH_TOZERO);

    // Localizing the best match with minMaxLoc
    double minVal;
    double maxVal;
    double accordance;

    cv::Point minLoc;
    cv::Point maxLoc;
    cv::Point matchLoc;
    cv::Point matchOffset;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all other methods higher values are best.
    if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
        matchLoc = minLoc;
        accordance = minVal;
    }
    else {
        matchLoc = maxLoc;
        accordance = maxVal;
    }

    matchOffset = cv::Point(matchLoc.x + compare_img.cols, matchLoc.y + compare_img.rows);

    return new Comparison(accordance, matchLoc, matchOffset);
}