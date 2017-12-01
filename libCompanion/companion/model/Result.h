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

#ifndef COMPANION_RESULT_H
#define COMPANION_RESULT_H

#include <string>
#include <companion/draw/Drawable.h>

namespace Companion::Model
{
    /**
     * Result model class to store object detection scoring.
     * @author Andreas Sekulski
     */
    class Result
    {

    public:

        /**
         * Constructor to create a result model.
         * @param scoring Scoring value from matching between 0 and 100 percent.
         * @param id Identificator from model.
         * @param model Drawable model to get position from model in image.
         */
        Result(int scoring, int id, Draw::Drawable *model);

        /**
         * Default destructor
         */
        virtual ~Result();

        /**
         * Return scoring value between 0 to 100 percent. 100 percent total match 0 percent no match.
         * @return Scoring value which contains with detected object in frame.
         */
        int getScoring() const;

        /**
         * Get identificator from object.
         * @return Identificator from detected object.
         */
        int getId() const;

        /**
         * Drawable model to get position from detected object.
         * @return Drawable model from detected object.
         */
        Draw::Drawable *getModel() const;

    private:

        /**
         * Score matching in percentage 0 - 100 percent.
         */
        int scoring;

        /**
         * ID from object.
         */
        int id;

        /**
         * Object detection result.
         */
        Companion::Draw::Drawable *model;
    };
}

#endif //COMPANION_RESULT_H
