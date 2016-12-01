//
// Created by andre on 27.11.2016.
//

#ifndef COMPANION_COMPARISON_H
#define COMPANION_COMPARISON_H

#include <opencv2/core/core.hpp>

using namespace cv;

class Comparison {

    public:
        Comparison(double accordance, Point location, Point offset);

        ~Comparison();

        double getAccordance();

        void setAccordance(double accordance);

        void setLocation(Point location);

        Point getLocation();

        void setOffset(Point offset);

        Point getOffset();

private:

        double accordance;

        Point location;

        Point offset;

};


#endif //COMPANION_COMPARISON_H
