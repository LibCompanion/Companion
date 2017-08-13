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

namespace Companion {

    namespace Model {

        /**
         * Result model class to store object detection scoring.
         * @author Andreas Sekulski
         */
        class Result {

        public:

            Result(int scoring, int id, Draw::Drawable *model);

            virtual ~Result();

            int getScoring() const;

            int getId() const;

            Draw::Drawable *getModel() const;

        private:

            /**
             * Score matching in percentage 0 - 100 %.
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
}

#endif //COMPANION_RESULT_H
