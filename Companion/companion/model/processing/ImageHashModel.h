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

#ifndef COMPANION_IMAGEHASHMODEL_H
#define COMPANION_IMAGEHASHMODEL_H

#include <vector>
#include <string>
#include <random>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <companion/util/Definitions.h>
#include <companion/util/exportapi/ExportAPIDefinitions.h>

namespace Companion { namespace Model { namespace Processing
{

    /**
     * Image hashing model to generate from images a hash representation.
     * @author Andreas Sekulski, Dimitri Kotlovsky
     */
    class COMP_EXPORTS ImageHashModel {

    public:

        /**
         * Constructor.
         */
        ImageHashModel();

        /**
         * Destructor.
         */
        virtual ~ImageHashModel();

        /**
         * Adds descriptor from given image.
         * @param id ID of the model.
         * @param descriptor Descriptor to add.
         */
        void addDescriptor(int id, cv::Mat &descriptor);

        /**
         * Generates dataset from current image hash model.
         * @return Generated dataset that contains as first element the hashed images and as second element the index dataset.
         */
        std::pair<cv::Mat_<float>, cv::Mat> generateDataset();

        /**
         * Returns the result scores.
         * @return Result scores.
         */
        const std::vector<std::pair<int, float>> &getScores() const;

    private:

        /**
         * Indicates whether a new model was added.
         */
        bool newModelAdded;

        /**
         * Raw image models to compare in an one dimensional array.
         */
        cv::Mat imageDataset;

        /**
         * Hash values to store from all models.
         */
        cv::Mat_<float> hash;

        /**
         * Index dataset from all models.
         */
        cv::Mat indexDataset;

        /**
         * Scores from all given models.
         */
        std::vector<std::pair<int, float>> scores;

        /**
         * Generates hash values from all image descriptors.
         * @return 1D Image hash matrix.
         */
        cv::Mat_<float> generateHashImages();

        /**
         * Generates index dataset from given hash.
         * @param hash Image hash to generate index dataset.
         * @return Index dataset from image hash.
         */
        cv::Mat generateIndexDataset(cv::Mat_<float> hash);
    };
}}}

#endif //COMPANION_IMAGEHASHMODEL_H
