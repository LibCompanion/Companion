/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2017 Andreas Sekulski
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMPANION_COMPARISON_H
#define COMPANION_COMPARISON_H

#include <opencv2/core/core.hpp>

/**
 * Comparison data model to store search results from an image recognition.
 * @author Andreas Sekulski
 */
class Comparison {

public:

    // ToDo := Redesign this model for an default search entity...

    /**
     * Comparison model to store accordance, location and offset position from search.
     * @param accordance
     * @param location
     * @param offset
     */
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