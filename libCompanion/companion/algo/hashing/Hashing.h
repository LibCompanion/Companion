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

#ifndef COMPANION_HASHING_H
#define COMPANION_HASHING_H

#include <companion/algo/ImageRecognition.h>
#include <companion/model/Result.h>
#include <companion/model/processing/ImageHashModel.h>
#include <companion/draw/Frame.h>

namespace Companion::Algorithm::Hashing
{
    class COMP_EXPORTS Hashing : public ImageRecognition
    {

        public:

            virtual Companion::Model::Result* executeAlgorithm(Companion::Model::Processing::ImageHashModel *model, 
                cv::Mat query, Companion::Draw::Frame *roi) = 0;

            /**
             * Indicator if this algorithm use cuda.
             * @return True if cuda will be used otherwise false for CPU/OpenCL usage.
             */
            virtual bool isCuda() = 0;
    };
}

#endif //COMPANION_HASHING_H
