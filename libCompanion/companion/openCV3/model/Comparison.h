//
// Created by andre on 27.11.2016.
//

#ifndef COMPANION_COMPARISON_H
#define COMPANION_COMPARISON_H

#include <opencv2/core/core.hpp>

class Comparison {

    public:
        Comparison(double accordance, cv::Point location, cv::Point offset);

        ~Comparison();

        double getAccordance();

        void setAccordance(double accordance);

        void setLocation(cv::Point location);

        cv::Point getLocation();

        void setOffset(cv::Point offset);

        cv::Point getOffset();

private:

        double accordance;

        cv::Point location;

        cv::Point offset;

};


#endif //COMPANION_COMPARISON_H
