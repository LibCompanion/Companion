/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2018 Andreas Sekulski
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

#ifndef COMPANION_RECOGNITION_H
#define COMPANION_RECOGNITION_H

#include <companion/util/exportapi/ExportAPIDefinitions.h>

namespace Companion { namespace Algorithm { namespace Recognition
{
	/**
	 * Recognition abstract class to implement specific recognition algorithms.
	 * @author Andreas Sekulski
	 */
	class COMP_EXPORTS Recognition
	{

	public:

		/**
		 * Indicator if this algorithm uses cuda.
		 * @return True if cuda will be used otherwise false for CPU/OpenCL usage.
		 */
		virtual bool isCuda() const = 0;
	};
}}}

#endif //COMPANION_RECOGNITION_H
