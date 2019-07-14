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

#include "HybridRecognition.h"

Companion::Processing::Recognition::HybridRecognition::HybridRecognition(PTR_HASH_RECOGNITION hashRecognition,
	PTR_FEATURE_MATCHING featureMatching, int resize)
{
	this->hashRecognition = hashRecognition;
	this->featureMatching = featureMatching;
	this->featureMatching->UseIRA(false);
	this->resize = resize;
}

void Companion::Processing::Recognition::HybridRecognition::AddModel(cv::Mat image, int id)
{
	PTR_MODEL_FEATURE_MATCHING model = std::make_shared<MODEL_FEATURE_MATCHING>();
	model->ID(id);
	model->Image(image);
	this->models[id] = model;
	this->hashRecognition->AddModel(id, image);
}

void Companion::Processing::Recognition::HybridRecognition::RemoveModel(int modelID)
{
	this->models.erase(modelID);
	// ToDo delete method for hashRecognition...
}

void Companion::Processing::Recognition::HybridRecognition::ClearModels()
{
	this->models.clear();
	// ToDo delete method for hashRecognition...
}

CALLBACK_RESULT Companion::Processing::Recognition::HybridRecognition::Execute(cv::Mat frame)
{
	CALLBACK_RESULT results;
	std::vector<CALLBACK_RESULT> parallelizedResults;
	cv::Mat roiFrame;
	std::vector<Companion::Error::Code> errors;
	PTR_RESULT hashResult;
	std::vector<PTR_RESULT> hashResults;
	int threads;

	threads = omp_get_max_threads();
	parallelizedResults = std::vector<CALLBACK_RESULT>(threads);
	hashResults = this->hashRecognition->Execute(frame);

	if (!hashResults.empty())
	{
		errors.clear();
#pragma omp parallel for
		for (int i = 0; i < hashResults.size(); i++)
		{
			try
			{
				hashResult = hashResults.at(i);
				if (hashResult != nullptr)
				{
					Processing(hashResult, frame, parallelizedResults[omp_get_thread_num()]);
				}
			}
			catch (Companion::Error::Code errorCode)
			{
#pragma omp critical
				errors.push_back(errorCode);
			}
		}

		if (!errors.empty())
		{
			throw Companion::Error::CompanionException(errors);
		}
	}

#pragma omp critical
	for (int i = 0; i < threads; i++)
	{
		for (int j = 0; j < parallelizedResults[i].size(); j++)
		{
			results.push_back(std::shared_ptr<RESULT>(parallelizedResults[i].at(j)));
		}
	}

	return results;
}

void Companion::Processing::Recognition::HybridRecognition::Processing(
	PTR_RESULT hashResult,
	cv::Mat frame,
	CALLBACK_RESULT& results)
{

	cv::Mat cutImage;
	PTR_DRAW cutDrawable;
	PTR_RESULT_RECOGNITION fmResult;
	PTR_MODEL_FEATURE_MATCHING sceneModel = std::make_shared<MODEL_FEATURE_MATCHING>();;
	int oldX, oldY;

	// This frame can be cut to improve recognition
	cutDrawable = hashResult->Drawable();
	cutImage = Companion::Util::CutImage(frame, cutDrawable->CutArea());

	oldX = cutImage.cols;
	oldY = cutImage.rows;

	if (this->resize != 100)
	{
		Util::ResizeImage(cutImage, cutImage.cols * this->resize / 100);
	}

	sceneModel->Image(cutImage);
	std::shared_ptr<RESULT_RECOGNITION> resultReco = std::dynamic_pointer_cast<RESULT_RECOGNITION>(hashResult);
	fmResult = this->featureMatching->ExecuteAlgorithm(sceneModel, this->models[resultReco->Id()], nullptr);

	if (fmResult != nullptr)
	{
		fmResult->Drawable()->Ratio(cutImage.cols, cutImage.rows, oldX, oldY);
		fmResult->Drawable()->MoveOrigin(cutDrawable->OriginX(), cutDrawable->OriginY());
		results.push_back(std::shared_ptr<RESULT>(fmResult));
	}
}
