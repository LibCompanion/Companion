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

#ifndef COMPANION_HASHDETECTION_H
#define COMPANION_HASHDETECTION_H

#include <companion/algo/2D/ShapeDetection.h>
#include <companion/processing/ImageProcessing.h>
#include <companion/model/processing/ImageHashModel.h>
#include <companion/util/Util.h>

namespace Companion {

	namespace Processing {

		class COMP_EXPORTS HashDetection : public ImageProcessing {

		public:

			HashDetection(cv::Size modelSize, Companion::Algorithm::ShapeDetection *shapeDetection);

			virtual ~HashDetection();

			/**
			 * Execution from given image processing algo implementation like face recognition or object detection.
			 * @param frame Obtained image frame from producer thread.
			 * @return  An empty vector if no objects are detected or otherwise a pair of a Drawable and the ID for
			 *          every detected object.
			 */
			virtual CALLBACK_RESULT execute(cv::Mat frame);

			bool addModel(int id ,cv::Mat image);

		private:

			cv::Size modelSize;

			Companion::Algorithm::ShapeDetection *shapeDetection;

			Companion::Model::Processing::ImageHashModel *model;

			std::vector<std::pair<int, float>> scores;

		};
	}
}


#endif //COMPANION_HASHDETECTION_H
