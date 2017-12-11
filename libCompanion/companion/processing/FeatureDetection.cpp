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

#include "FeatureDetection.h"

Companion::Processing::FeatureDetection::FeatureDetection(Companion::Algorithm::Matching::Matching *matchingAlgo,
	Companion::SCALING scaling,
	Companion::Algorithm::Detection::ShapeDetection *shapeDetection)
{
	this->matchingAlgo = matchingAlgo;
	this->scaling = scaling;
	this->shapeDetection = shapeDetection;
}

Companion::Processing::FeatureDetection::~FeatureDetection()
{
}

CALLBACK_RESULT Companion::Processing::FeatureDetection::execute(cv::Mat frame)
{
	Companion::Algorithm::Matching::FeatureMatching *featureMatching;
	Companion::Model::Processing::FeatureMatchingModel *sceneModel;
	CALLBACK_RESULT objects;
	std::vector<Companion::Draw::Frame*> rois;
	int oldX, oldY;

	if (!frame.empty())
	{
		sceneModel = new Companion::Model::Processing::FeatureMatchingModel();

		oldX = frame.cols;
		oldY = frame.rows;

		// Shrink the image with a given scale factor or a given output width. Use this list for good 16:9 image sizes:
		// https://antifreezedesign.wordpress.com/2011/05/13/permutations-of-1920x1080-for-perfect-scaling-at-1-77/
		Util::resizeImage(frame, this->scaling);
		sceneModel->setImage(frame);

		featureMatching = dynamic_cast<Companion::Algorithm::Matching::FeatureMatching*>(matchingAlgo);
		if (featureMatching != nullptr)
		{
			// matching algorithm is feature matching.
			// Pre calculate full image scene model keypoints
			featureMatching->calculateKeyPoints(sceneModel);
		}

		if (shapeDetection != nullptr)
		{
			// If shape detection should be used obtain all posible rois from frame.
			rois = shapeDetection->executeAlgorithm(sceneModel->getImage());
		}

		if (matchingAlgo->isCuda())
		{
			for (int x = 0; x < models.size(); x++)
			{
				processing(sceneModel,
					models.at(x),
					rois,
					frame,
					oldX,
					oldY,
					objects);
			}
		}
		else
		{
			#pragma omp parallel for
			for (int x = 0; x < models.size(); x++)
			{
				processing(sceneModel,
					models.at(x),
					rois,
					frame,
					oldX,
					oldY,
					objects);
			}
		}

		frame.release();
		delete sceneModel;
	}

	return objects;
}

void Companion::Processing::FeatureDetection::processing(Companion::Model::Processing::FeatureMatchingModel* sceneModel,
    Companion::Model::Processing::FeatureMatchingModel* objectModel,
	std::vector<Companion::Draw::Frame*> rois,
	cv::Mat frame,
	int originalX,
	int originalY,
	CALLBACK_RESULT &objects)
{
	Companion::Model::Result* result = nullptr;

	if (!objectModel)
	{
		// If wrong model types are used
		throw Companion::Error::Code::wrong_model_type;
	}

	if (rois.size() == 0)
	{
		// If rois not found or used
		result = matchingAlgo->executeAlgorithm(sceneModel, objectModel, nullptr);
	}
	else
	{
		int index = 0;
		// If rois found
		while (index < rois.size())
		{
			result = matchingAlgo->executeAlgorithm(sceneModel, objectModel, rois.at(index));
			index++;
		}
	}

	if (result != nullptr)
	{
		// Create old image size
		result->getModel()->ratio(frame.cols, frame.rows, originalX, originalY);
		// Store detected object and its ID to vector.
		objects.push_back(result);
	}
}

bool Companion::Processing::FeatureDetection::addModel(Companion::Model::Processing::FeatureMatchingModel *model)
{

	if (!model->getImage().empty())
	{
		this->models.push_back(model);
		return true;
	}

	return false;
}

bool Companion::Processing::FeatureDetection::removeModel(Companion::Model::Processing::FeatureMatchingModel *model)
{
	for (int index = 0; index < this->models.size(); index++)
	{
		if (this->models.at(index)->getID() == model->getID()) {
			this->models.erase(this->models.begin() + index);
			return true;
		}
	}
	return false;
}

void Companion::Processing::FeatureDetection::clearModels()
{
	this->models.clear();
}
