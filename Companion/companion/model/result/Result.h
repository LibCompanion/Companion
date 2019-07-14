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

 /// @file
#ifndef COMPANION_RESULT_H
#define COMPANION_RESULT_H

#include <string>
#include <companion/draw/Drawable.h>
#include <companion/util/Definitions.h>

namespace Companion {
	namespace Model {
		namespace Result
		{
			/**
			 * Result types.
			 */
			enum class ResultType {
				RECOGNITION, ///< Object recognition result.
				DETECTION ///< Object detection result.
			};

			/**
			 * Result model abstract class to store object detection or image recognition results.
			 * @author Andreas Sekulski, Dimitri Kotlovsky
			 */
			class COMP_EXPORTS Result
			{

			public:

				/**
				 * Constructor to create a result model.
				 * @param scoring Scoring value from matching between 0 and 100 percent.
				 * @param drawable Drawable result that shows the location of the detected or recognized object in the image.
				 */
				Result(int scoring, PTR_DRAW drawable);

				/**
				 * Return scoring value between 0 to 100 percent. 100% means total match, 0% means no match.
				 * @return Scoring value of the detected or recognized object.
				 */
				int Scoring() const;

				/**
				 * Return the drawable result that shows the location of the detected or recognized object in the image.
				 * @return Drawable result.
				 */
				PTR_DRAW Drawable() const;

				/**
				 * Return object description. This may be the ID or the object type.
				 * @return Object description.
				 */
				virtual std::string Description() const = 0;

				/**
				 * Return the type of this result.
				 * @return Result type.
				 */
				virtual ResultType Type() const = 0;

			private:

				/**
				 * Scoring value between 0 to 100 percent.
				 */
				int scoring;

				/**
				 * Drawable result.
				 */
				PTR_DRAW drawable;
			};
		}
	}
}

#endif //COMPANION_RESULT_H
