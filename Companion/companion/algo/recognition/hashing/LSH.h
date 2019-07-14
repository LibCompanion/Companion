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

#ifndef COMPANION_LSH_H
#define COMPANION_LSH_H

#include "Hashing.h"

namespace Companion {
	namespace Algorithm {
		namespace Recognition {
			namespace Hashing
			{
				/**
				 * Local Sensitive Hashing (LSH) implementation for hash image comparison.
				 * @author Andreas Sekulski, Dimitri Kotlovsky
				 */
				class COMP_EXPORTS LSH : public Hashing {

				public:

					/**
					 * LSH algorithm execution method to compare an image hash model with a query.
					 * @param model Image hash model to compare.
					 * @param query Query image to compare with hash model.
					 * @param roi Region of interest to check.
					 * @return Nullptr if no matching success otherwise a recognition result.
					 */
					PTR_RESULT_RECOGNITION ExecuteAlgorithm(PTR_MODEL_IMAGE_HASHING model, cv::Mat query, PTR_DRAW_FRAME roi);

					/**
					 * Indicator if this algorithm uses cuda.
					 * @return True if cuda will be used otherwise false for CPU/OpenCL usage.
					 */
					bool IsCuda() const;
				};
			}
		}
	}
}

#endif //COMPANION_LSH_H
