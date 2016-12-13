#ifndef COMPANION_SIMPLECOMPAREIMAGE_H
#define COMPANION_SIMPLECOMPAREIMAGE_H

#include "ImageRecognition.h"

/**
 * Simple compare image implementation to verify two images for equality.
 *
 * @author Andreas Sekulski
 */
class SimpleCompareImage : public ImageRecognition {

    public:

        /**
         * Default constructor.
         */
        SimpleCompareImage();

        /**
         * Default destructor.
         */
        ~SimpleCompareImage();

    protected:

        /**
         * Simple search compares compare_img_path for similarity.<br> Best matches are high results and approach goes to one.
         *
         * @param search_img Search image to check.
         * @param compare_img Compare image to equal search image.
         *
         * @throws CompanionError::error_code If an error occured in search operation like dimension not equal.
         *
         * @return Accordance from equality from 0 image not equal and 1 image are equal.
         */
        virtual Comparison* algo(cv::Mat search_img, cv::Mat compare_img);
};

#endif //COMPANION_SIMPLECOMPAREIMAGE_H