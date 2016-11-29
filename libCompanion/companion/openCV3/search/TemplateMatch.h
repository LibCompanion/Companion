#ifndef COMPANION_TEMPLATEMATCH_H
#define COMPANION_TEMPLATEMATCH_H

#include "ImageRecognition.h"

/**
 * Template matching image recognition implementation from <a href="http://docs.opencv.org/3.1.0/de/da9/tutorial_template_matching.html">OpenCV</a>.
 *
 * @author Andreas Sekulski
 */
class TemplateMatch : public ImageRecognition {

    public:

        /**
         * Default constrcutor set match_method to SQDIFF_NORMED.
         */
        TemplateMatch();

         /**
          * Constructor to create an template match algo search.
          *
          * @param match_method Template matching methods like CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED.
          */
        TemplateMatch(int match_method);

        /**
         * Default constructor.
         */
        ~TemplateMatch();

        /**
         * Set template matching method like CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED.
         * @param match_method Match method to set for template matching.
         */
        void set_match_method(int match_method);

    protected:

        /**
         * Template matching search implementation from <a href="http://docs.opencv.org/3.1.0/de/da9/tutorial_template_matching.html">OpenCV</a>.<br>
	     * Results depends on match_method.
         *
         * @param search_img Search image to check.
         * @param compare_img Compare image to equal search image.
         *
         * @throws CompanionError::error_code If an error occured in search operation.
         *
         * @return Results depends on match_method. <br>
         *         For SQDIFF and SQDIFF_NORMED, the best matches are lower values (0).<br>
         *         All other methods higher values are best matches (1)
         */
        virtual Comparison* algo(Mat search_img, Mat compare_img);

    private:

        /**
         * Template matching methods like CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM_CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED.
         */
        int match_method;
};

#endif //COMPANION_TEMPLATEMATCH_H