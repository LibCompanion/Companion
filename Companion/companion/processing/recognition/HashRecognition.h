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

#ifndef COMPANION_HASHRECOGNITION_H
#define COMPANION_HASHRECOGNITION_H

#include <companion/processing/ImageProcessing.h>
#include <companion/algo/detection/ShapeDetection.h>
#include <companion/model/processing/ImageHashModel.h>
#include <companion/algo/recognition/hashing/Hashing.h>
#include <companion/util/Util.h>

namespace Companion { namespace Processing { namespace Recognition
{
    /**
     * Hash recognition implementation to recognize objects based on hash values.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS HashRecognition : public ImageProcessing
    {

    public:

        /**
         * Hash recognition construtor.
         * @param modelSize Model size in pixel.
         * @param shapeDetection Shape detection algorithm to detect ROI's.
         * @param hashing Hashing algorithm implementation, for example LSH.
         */
        HashRecognition(cv::Size modelSize,
            Companion::Algorithm::Detection::ShapeDetection *shapeDetection,
            Companion::Algorithm::Recognition::Hashing::Hashing *hashing);

        /**
         * Default destructor.
         */
        virtual ~HashRecognition();

        /**
         * Adds a model to search.
         * @param id Identity from model.
         * @param image Image from model to store as hash.
         * @return TRUE if model is added otherwise false.
         */
        bool addModel(int id, cv::Mat image);

        /**
         * Try to recognize all objects in the given frame.
         * @param frame Frame to check for an object location.
         * @return  An empty vector if no objects are recognized or otherwise a pair of a Drawable and the ID for
         *          every recognized object.
         */
        CALLBACK_RESULT execute(cv::Mat frame);

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
         * Models to recognize.
         */
        Companion::Model::Processing::ImageHashModel *model;

        /**
         * Stores hashing algorithm to recognized objects.
         */
        Companion::Algorithm::Recognition::Hashing::Hashing *hashing;
    };
}}}

#endif //COMPANION_HASHRECOGNITION_H
