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

#include <companion/algo/detection/ShapeDetection.h>
#include <companion/processing/ImageProcessing.h>
#include <companion/model/processing/ImageHashModel.h>
#include <companion/algo/hashing/Hashing.h>
#include <companion/util/Util.h>

namespace Companion { namespace Processing
{

    /**
     * Hash detection implementation to detect object from a image with hash values.
     */
    class COMP_EXPORTS HashDetection : public ImageProcessing {

    public:

        /**
         * Hash detection construtor to create a object detector.
         * @param modelSize Model size in pixel.
         * @param shapeDetection Shape detection algorithm to detect ROI's.
         * @param hashing Hashing algorithm implementation for example LSH.
         */
        HashDetection(cv::Size modelSize, 
            Companion::Algorithm::Detection::ShapeDetection *shapeDetection,
            Companion::Algorithm::Hashing::Hashing *hashing);

        /**
         * Default destructor.
         */
        virtual ~HashDetection();

        /**
         * Execution from given image processing algo implementation like face recognition or object detection.
         * @param frame Obtained image frame from producer thread.
         * @return  An empty vector if no objects are detected or otherwise a pair of a Drawable and the ID for
         *          every detected object.
         */
        virtual CALLBACK_RESULT execute(cv::Mat frame);

        /**
         * Adds a model to search.
         * @param id Identity from model.
         * @param image Image from model to store as hash.
         * @return TRUE if model is added otherwise false.
         */
        bool addModel(int id, cv::Mat image);

    private:

        /**
         * Stores model size in pixel.
         */
        cv::Size modelSize;

        /**
         * Stores shape detection algorithm to search for roi's.
         */
        Companion::Algorithm::Detection::ShapeDetection *shapeDetection;

        /**
         * Models to detect.
         */
        Companion::Model::Processing::ImageHashModel *model;

        /**
         * Stores hashing algorithm to detect objects.
         */
        Companion::Algorithm::Hashing::Hashing *hashing;
    };
}}

#endif //COMPANION_HASHDETECTION_H
