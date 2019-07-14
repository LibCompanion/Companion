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

#ifndef COMPANION_HYBRIDRECOGNITION_H
#define COMPANION_HYBRIDRECOGNITION_H

#include <companion/processing/ImageProcessing.h>
#include <companion/processing/recognition/HashRecognition.h>
#include <companion/algo/detection/ShapeDetection.h>
#include <companion/algo/recognition/matching/FeatureMatching.h>
#include <companion/model/processing/FeatureMatchingModel.h>
#include <companion/util/CompanionException.h>
#include <omp.h>

namespace Companion {
	namespace Processing {
		namespace Recognition
		{
			/**
			 * Hybrid recognition implementation to recognize objects based on hash values and verify with a matching algorithm (for example feature matching).
			 * @author Andreas Sekulski, Dimitri Kotlovsky
			 */
			class HybridRecognition : public ImageProcessing
			{

			public:

				/**
				 * Hybrid recognition constructor.
				 * @param hashRecognition Hash recognition to use.
				 * @param featureMatching Feature matching to verify recognized hashes.
				 * @param resize Resize image factor from 1 to 100 (in percent). 100 is equal to 100% of the original scale.
				 */
				HybridRecognition(PTR_HASH_RECOGNITION hashRecognition,
					PTR_FEATURE_MATCHING featureMatching,
					int resize = 100);

				/**
				 * Destructor.
				 */
				virtual ~HybridRecognition() = default;

				/**
				 * Add search model type to search for.
				 * @param image Image model to search for.
				 * @param id Identifier of the model.
				 */
				void AddModel(cv::Mat image, int id);

				/**
				 * Remove given model if it exists. This method can only be used safely if the searching process is not running.
				 * @param modelID ID of the model to remove.
				 */
				void RemoveModel(int modelID);

				/**
				 * Clear all models which are searched for.
				 */
				void ClearModels();

				/**
				 * Try to recognize all objects in the given frame.
				 * @param frame Frame to check for an object location.
				 * @return A vector of results for the given frame or an empty vector if no objects are recognized.
				 */
				CALLBACK_RESULT Execute(cv::Mat frame);

			private:

				/**
				 * Resize factor for recognized hash model images.
				 */
				int resize;

				/**
				 * Hash recognition.
				 */
				PTR_HASH_RECOGNITION hashRecognition;

				/**
				 * Feature matching algorithm to use for model verification.
				 */
				PTR_FEATURE_MATCHING featureMatching;

				/**
				 * A map of all models to recognize.
				 */
				std::map<int, PTR_MODEL_FEATURE_MATCHING> models;

				/**
				 * Processing method to recognize objects.
				 * @param hashResult Result from hash recognition.
				 * @param frame Scene frame.
				 * @param results List of all recognized objects.
				 */
				void Processing(PTR_RESULT hashResult,
					cv::Mat frame,
					CALLBACK_RESULT& results);

			};
		}
	}
}

#endif //COMPANION_HYBRIDRECOGNITION_H
