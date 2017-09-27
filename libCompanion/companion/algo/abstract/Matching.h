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

#ifndef COMPANION_ABSTRACTFEATUREMATCHING_H
#define COMPANION_ABSTRACTFEATUREMATCHING_H

#include "ImageRecognition.h"

#include <companion/model/result/Result.h>
#include <companion/draw/Frame.h>
#include <companion/model/processing/FeatureMatchingModel.h>

namespace Companion
{

	namespace Algorithm
	{

		/**
		 * Abstract matching class implementation for all default matching algorithms like feature matching or template matching.
		 * @author Andreas Sekulski
		 */
		class COMP_EXPORTS Matching : public ImageRecognition
		{

		public:

			/**
			 * Feature matching algorithm implementation to search in an scene model this given object model.
			 * @param sceneModel Scene model to verify for matching.
			 * @param objectModel Object model to search in scene.
			 * @param roi A region of interest to detect for searched for object if nullptr not used.
			 * @return An result model if an object is detected otherwise nullptr.
			 */
			virtual Companion::Model::Result* executeAlgorithm(Model::Processing::FeatureMatchingModel *sceneModel,
				Model::Processing::FeatureMatchingModel *objectModel,
				Companion::Draw::Frame *roi) = 0;

			/**
			 * Indicator if this algorithm use cuda.
			 * @return True if cuda will be used otherwise false.
			 */
			virtual bool isCuda() = 0;
		};
	}
}

#endif //COMPANION_ABSTRACTFEATUREMATCHING_H
