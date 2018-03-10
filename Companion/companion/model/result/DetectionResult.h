/*
 * This program is an object recognition framework written with OpenCV.
 * Copyright (C) 2016-2018 Andreas Sekulski, Dimitri Kotlovsky
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

#ifndef COMPANION_DETECTIONRESULT_H
#define COMPANION_DETECTIONRESULT_H

#include <companion/model/result/Result.h>

namespace Companion { namespace Model { namespace Result
{
    /**
     * Result model class to store object detection results.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS DetectionResult : public Result
    {

    public:

        /**
         * Constructor to create a detection result model.
         * @param scoring Scoring value from matching between 0 and 100 percent.
         * @param objectType Object type.
         * @param drawable Drawable result that shows the location of the detected object in the image.
         */
        DetectionResult(int scoring, std::string objectType, Draw::Drawable *drawable);

        /**
         * Default destructor
         */
        virtual ~DetectionResult();

        /**
         * Return object type.
         * @return Object type.
         */
        std::string getObjectType() const;

        /**
         * Return object description.
         * @return Object description.
         */
        std::string getDescription() const;

        /**
         * Return the type of this result.
         * @return Result type.
         */
        ResultType getType() const;

    private:

        /**
         * Object type.
         */
        std::string objectType;
    };
}}}

#endif //COMPANION_DETECTIONRESULT_H
