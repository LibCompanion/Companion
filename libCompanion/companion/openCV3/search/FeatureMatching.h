//
// Created by andre on 28.11.2016.
//

#ifndef COMPANION_FEATUREMATCHING_H
#define COMPANION_FEATUREMATCHING_H

#include "ImageRecognition.h"

namespace FeatureMatcher {

    enum class detector {
        AKAZE,
        BRISK,
        FastFeatureDetector,
        GFTTDetector,
        KAZE,
        MSER,
        ORB,
        SimpleBlobDetector
    };

    enum class matcher {
        BruteForce_L2,
        BruteForce_L1,
        BruteForce_Hamming,
        BruteForce_Hamming_2,
        FlannBased
    };
}

/**
 * Feature matching algo implementation from <a href="http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html">OpenCV</a>.
 *
 * @author Andreas Sekulski
 */
class FeatureMatching : public ImageRecognition {

    public:
        FeatureMatching();

        FeatureMatching(FeatureMatcher::detector detector, FeatureMatcher::matcher);

        ~FeatureMatching();

    private:

        FeatureMatcher::detector detector;

        FeatureMatcher::matcher matcher;

        /**
         * ToDo Documentation
         */
        Ptr<Feature2D> get_feature_detector(FeatureMatcher::detector f_detector);

        /**
         * ToDo Documentation
         */
        string get_decriptor_matcher(FeatureMatcher::matcher f_matcher);

    protected:
        virtual Comparison* algo(Mat search_img, Mat compare_img);
};


#endif //COMPANION_FEATUREMATCHING_H
