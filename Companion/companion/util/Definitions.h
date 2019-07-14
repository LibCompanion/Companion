/*
 * This program is an image recognition library written with OpenCV.
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

 /// @file
#ifndef COMPANION_DEFINITIONS_H
#define COMPANION_DEFINITIONS_H

#include <memory>

namespace Companion
{
	// Companion
	#define COMPANION Companion::Configuration
	#define PTR_COMPANION std::shared_ptr<COMPANION>

	// Detection definitions
	#define SHAPE_DETECTION Companion::Algorithm::Detection::ShapeDetection
	#define PTR_SHAPE_DETECTION std::shared_ptr<SHAPE_DETECTION>

	#define OBJECT_DETECTION Companion::Processing::Detection::ObjectDetection
	#define PTR_OBJECT_DETECTION std::shared_ptr<OBJECT_DETECTION>

	// Thread worker definitions
	#define STREAM_WORKER Companion::Thread::StreamWorker
	#define PTR_STREAM_WORKER std::shared_ptr<STREAM_WORKER>

	// Stream module definitions
	#define STREAM Companion::Input::Stream
	#define PTR_STREAM std::shared_ptr<STREAM>

	#define VIDEO_STREAM Companion::Input::Video
	#define PTR_VIDEO_STREAM std::shared_ptr<VIDEO_STREAM>

	#define IMAGE_STREAM Companion::Input::Image
	#define PTR_IMAGE_STREAM std::shared_ptr<IMAGE_STREAM>

	// Image processing definitions
	#define IMAGE_PROCESSING Companion::Processing::ImageProcessing
	#define PTR_IMAGE_PROCESSING std::shared_ptr<IMAGE_PROCESSING>

	#define MATCH_RECOGNITION Companion::Processing::Recognition::MatchRecognition
	#define PTR_MATCH_RECOGNITION std::shared_ptr<MATCH_RECOGNITION>

	#define HYBRID_RECOGNITION Companion::Processing::Recognition::HybridRecognition
	#define PTR_HYBRID_RECOGNITION std::shared_ptr<HYBRID_RECOGNITION>

	#define HASH_RECOGNITION Companion::Processing::Recognition::HashRecognition
	#define PTR_HASH_RECOGNITION std::shared_ptr<HASH_RECOGNITION>

	// Algorithm definitions
	#define MATCHING_RECOGNITION Companion::Algorithm::Recognition::Matching::Matching
	#define PTR_MATCHING_RECOGNITION std::shared_ptr<MATCHING_RECOGNITION>

	#define IMAGE_REDUCTION_ALGORITHM Companion::Algorithm::Recognition::Matching::IRA
	#define PTR_IMAGE_REDUCTION_ALGORITHM std::shared_ptr<IMAGE_REDUCTION_ALGORITHM>

	#define FEATURE_MATCHING Companion::Algorithm::Recognition::Matching::FeatureMatching
	#define PTR_FEATURE_MATCHING std::shared_ptr<FEATURE_MATCHING>

	#define HASHING Companion::Algorithm::Recognition::Hashing::Hashing
	#define PTR_HASHING std::shared_ptr<HASHING>

	#define HASHING_LSH Companion::Algorithm::Recognition::Hashing::LSH
	#define PTR_HASHING_LSH std::shared_ptr<HASHING_LSH>

	// Model definitions
	#define MODEL_FEATURE_MATCHING Companion::Model::Processing::FeatureMatchingModel
	#define PTR_MODEL_FEATURE_MATCHING std::shared_ptr<MODEL_FEATURE_MATCHING>

	#define MODEL_IMAGE_HASHING Companion::Model::Processing::ImageHashModel
	#define PTR_MODEL_IMAGE_HASHING std::shared_ptr<MODEL_IMAGE_HASHING>

	// Draw model definitions
	#define DRAW Companion::Draw::Drawable
	#define PTR_DRAW std::shared_ptr<DRAW>

    #define DRAW_FRAME Companion::Draw::Frame
	#define PTR_DRAW_FRAME std::shared_ptr<DRAW_FRAME>

	#define DRAW_LINE Companion::Draw::Line

	// Result definitions
	#define RESULT Companion::Model::Result::Result
	#define PTR_RESULT std::shared_ptr<RESULT>

	#define RESULT_RECOGNITION Companion::Model::Result::RecognitionResult
	#define PTR_RESULT_RECOGNITION std::shared_ptr<RESULT_RECOGNITION>

	#define RESULT_DETECTION Companion::Model::Result::DetectionResult
	#define PTR_RESULT_DETECTION std::shared_ptr<RESULT_DETECTION>


    /**
     * Callback result definition.
     */
    #define CALLBACK_RESULT std::vector<PTR_RESULT>

     /**
      * Default success callback function declaration to obtain results from algorithms.
      */
    #define SUCCESS_CALLBACK void(CALLBACK_RESULT, cv::Mat)

      /**
       * Default error callback function declaration to obtain error results from companion.
       */
    #define ERROR_CALLBACK void(Companion::Error::Code)
}

#endif //COMPANION_DEFINITIONS_H
