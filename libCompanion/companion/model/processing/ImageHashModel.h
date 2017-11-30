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

#ifndef COMPANION_IMAGEHASHMODEL_H
#define COMPANION_IMAGEHASHMODEL_H

#include <companion\util\Definitions.h>
#include <vector>
#include <string>
#include <random>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace Companion {

    namespace Model {

        namespace Processing {

			/**
			 * Image hashing model to generate from images a hash representation.
			 * @author Andreas Sekulski
			 */
            class COMP_EXPORTS ImageHashModel {

			public:

                /**
                 * Default constructor.
                 */
				ImageHashModel();

                /**
                 * Default destructor.
                 */
                virtual ~ImageHashModel();

                /**
                 * Add descriptor from given image.
                 * @param descriptor Descriptor to add.
                 */
				void addDescriptor(cv::Mat &descriptor);

                /**
                 * Generates dataset from current image hash model.
                 * @return Generated dataset contains in first element hashed images and second index dataset.
                 */
                std::pair<cv::Mat_<float>, cv::Mat> generateDataset();

			private:

				bool newModelAdded;

				/**
				 * RAWR image models to compare in a 1 dimensional array.
				 **/
				cv::Mat imageDataset;

				cv::Mat_<float> hash;

				cv::Mat indexDataset;

                /**
                 * Generates from all image descriptors hash values.
                 * @return 1D Image hashes matrix.
                 */
                cv::Mat_<float> generateHashImages();

                /**
                 * Generates index dataset from given hash.
                 * @param hash Hash images to generate index dataset.
                 * @return Index dataset from hash image.
                 */
                cv::Mat generateIndexDataset(cv::Mat_<float> hash);
            };
        }
    }
}


#endif //COMPANION_IMAGEHASHMODEL_H
