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

#ifndef COMPANION_RECOGNITIONRESULT_H
#define COMPANION_RECOGNITIONRESULT_H

#include <companion/model/result/Result.h>

namespace Companion { namespace Model { namespace Result
{
    /**
     * Recognition result model class to store image recognition results.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS RecognitionResult : public Result
    {

    public:

        /**
         * Constructor to create a recognition result model.
         * @param scoring Scoring value from matching between 0 and 100 percent.
         * @param id Model ID.
         * @param drawable Drawable result that shows the location of the recognized object in the image.
         */
        RecognitionResult(int scoring, int id, Draw::Drawable *drawable);

        /**
         * Default destructor
         */
        virtual ~RecognitionResult();

        /**
         * Return object ID.
         * @return Object ID.
         */
        int getId() const;

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
         * Object ID.
         */
        int id;
    };
}}}

#endif //COMPANION_RECOGNITIONRESULT_H
