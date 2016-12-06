//
// Created by andre on 28.11.2016.
//

#ifndef COMPANION_FEATUREMATCHING_H
#define COMPANION_FEATUREMATCHING_H

#include "ImageRecognition.h"
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;

/**
 * Feature matching algo implementation from <a href="http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html">OpenCV</a>.
 *
 * @author Andreas Sekulski
 */
class FeatureMatching : public ImageRecognition {

public:
    FeatureMatching();

    FeatureMatching(Ptr<Feature2D> feature, Ptr<DescriptorMatcher> descriptor);

    ~FeatureMatching();

    const Ptr<Feature2D> &getFeature() const;

    void setFeature(const Ptr<Feature2D> &feature);

    const Ptr<DescriptorMatcher> &getDescriptor() const;

    void setDescriptor(const Ptr<DescriptorMatcher> &descriptor);

private:

    Ptr<Feature2D> feature;

    Ptr<DescriptorMatcher> descriptor;

protected:
    virtual Comparison *algo(Mat search_img, Mat compare_img);
};


#endif //COMPANION_FEATUREMATCHING_H
