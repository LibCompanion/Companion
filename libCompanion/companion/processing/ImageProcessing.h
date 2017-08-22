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

#ifndef COMPANION_IMAGEPROCESSING_H
#define COMPANION_IMAGEPROCESSING_H

#include <opencv2/core/core.hpp>
#include <companion/util/Definitions.h>

namespace Companion 
{

    namespace Processing 
	{

        /**
         * Image processing interface class to create specific image processing jobs to detect objects or regions of interests.
         *
         * @author Andreas Sekulski
         */
        class COMP_EXPORTS ImageProcessing 
		{

        public:

            /**
             * Execution from given image processing algo implementation like face recognition or object detection.
             * @param frame Obtained image frame from producer thread.
             * @return  An empty vector if no objects are detected or otherwise a pair of a Drawable and the ID for
             *          every detected object.
             */
            virtual CALLBACK_RESULT execute(cv::Mat frame) = 0;
        };

    }

}

#endif //COMPANION_IMAGEPROCESSING_H