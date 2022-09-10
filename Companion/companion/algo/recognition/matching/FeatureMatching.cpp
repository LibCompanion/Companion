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

#include "FeatureMatching.h"

Companion::Algorithm::Recognition::Matching::FeatureMatching::FeatureMatching(
	cv::Ptr<cv::FeatureDetector> detector,
	cv::Ptr<cv::DescriptorExtractor> extractor,
	cv::Ptr<cv::DescriptorMatcher> matcher,
	int matcherType,
	int minSidelLength,
	int countMatches,
	bool useIRA,
	double reprojThreshold,
	int ransacMaxIters,
	int findHomographyMethod)
{
	this->detector = detector;
	this->extractor = extractor;
	this->matcherType = matcherType;
	this->matcher = matcher;
	this->minSidelLength = minSidelLength;
	this->countMatches = countMatches;
	this->useIRA = useIRA;
	this->cudaUsed = false;
	this->reprojThreshold = reprojThreshold;
	this->ransacMaxIters = ransacMaxIters;
	this->findHomographyMethod = findHomographyMethod;
}

#if Companion_USE_CUDA
Companion::Algorithm::Recognition::Matching::FeatureMatching::FeatureMatching(
	cv::Ptr<cv::Feature2D> cudaFeatureMatching,
	int minSidelLength,
	int countMatches,
	double reprojThreshold,
	int ransacMaxIters,
	int findHomographyMethod)
{

	this->cudaFeatureMatching = cudaFeatureMatching;
	this->minSidelLength = minSidelLength;
	this->countMatches = countMatches;
	this->useIRA = false;
	this->cudaUsed = true;
	this->reprojThreshold = reprojThreshold;
	this->ransacMaxIters = ransacMaxIters;
	this->findHomographyMethod = findHomographyMethod;
}
#endif

#if Companion_USE_XFEATURES_2D && Companion_USE_CUDA
Companion::Algorithm::Recognition::Matching::FeatureMatching::FeatureMatching(cv::cuda::SURF_CUDA cudaFeatureMatching,
	int minSidelLength, int countMatches,
	double reprojThreshold,
	int ransacMaxIters,
	int findHomographyMethod) {

	this->surf_cuda = cudaFeatureMatching;
	this->minSidelLength = minSidelLength;
	this->countMatches = countMatches;
	this->useIRA = false;
	this->cudaUsed = true;
	this->reprojThreshold = reprojThreshold;
	this->ransacMaxIters = ransacMaxIters;
	this->findHomographyMethod = findHomographyMethod;
}
#endif

PTR_RESULT_RECOGNITION Companion::Algorithm::Recognition::Matching::FeatureMatching::ExecuteAlgorithm(
	PTR_MODEL_FEATURE_MATCHING sceneModel,
	PTR_MODEL_FEATURE_MATCHING objectModel,
	PTR_DRAW_FRAME roi)
{

	// Set of variables for feature matching
	cv::Mat sceneImage, objectImage;
	std::vector<std::vector<cv::DMatch>> matches;
	std::vector<cv::DMatch> goodMatches;
	std::vector<cv::KeyPoint> keypointsScene, keypointsObject;
	cv::Mat descriptorsScene, descriptorsObject;
	PTR_RESULT_RECOGNITION result = nullptr;
	PTR_DRAW drawable = nullptr;
	bool isIRAUsed = false;
	bool isROIUsed = false;
	PTR_IMAGE_REDUCTION_ALGORITHM ira;

	// Clear all lists from last run
	matches.clear();
	goodMatches.clear();
	keypointsScene.clear();
	keypointsObject.clear();

	sceneImage = sceneModel->Image(); // Get image scene
	objectImage = objectModel->Image(); // Get object scene
	ira = objectModel->Ira();  // Get IRA from object model

	// Check if images are loaded...
	if (!Util::IsImageLoaded(sceneImage) || !Util::IsImageLoaded(objectImage))
	{
		throw Companion::Error::Code::image_not_found;
	}

	cvtColor(sceneImage, sceneImage, cv::COLOR_BGR2GRAY); // Convert image to grayscale

	// --------------------------------------------------
	// Scene and model preparation start
	// --------------------------------------------------

	// ------ IRA scene handling. Currently works only for CPU usage ------
	if (this->useIRA && ira->IsObjectRecognized()) // IRA USED & OBJECT RECOGNIZED
	{
		// Cut out scene from last recognized object and set this as new scene to check
		sceneImage = cv::Mat(sceneImage, ira->LastObjectPosition());

		// Detect keypoints from cut scene
		this->detector->detect(sceneImage, keypointsScene);
		// Calculate descriptors from cut scene (feature vectors)
		this->extractor->compute(sceneImage, keypointsScene, descriptorsScene);

		isIRAUsed = true;
	}
	else if (this->useIRA && roi != nullptr) // IRA USED & OBJECT NOT RECOGNIZED & ROI EXISTS
	{
		// Get ROI position
		cv::Rect roiObject(roi->TopLeft(), roi->BottomRight());

		// Cut out scene from last recognized object and set this as new scene to check
		sceneImage = cv::Mat(sceneImage, roiObject);

		// Detect keypoints from cut scene
		this->detector->detect(sceneImage, keypointsScene);
		// Calculate descriptors from cut scene (feature vectors)
		this->extractor->compute(sceneImage, keypointsScene, descriptorsScene);

		isROIUsed = true;
	}
	else if (this->useIRA && sceneModel->KeypointsCalculated()) // IRA USED & OBJECT NOT RECOGNIZED & SCENE KEYPOINTS CALCULATED
	{
		keypointsScene = sceneModel->Keypoints();
		descriptorsScene = sceneModel->Descriptors();
	}
	else if (!this->cudaUsed) // IRA NOT USED & OBJECT NOT RECOGNIZED & SCENE KEYPOINTS NOT CALCULATED & NOT CUDA USAGE
	{
		// Detect keypoints
		this->detector->detect(sceneImage, keypointsScene);
		// Calculate descriptors
		this->extractor->compute(sceneImage, keypointsScene, descriptorsScene);
	}

	// Check if object has calculated keypoints and descriptors and CUDA is not used
	if (!objectModel->KeypointsCalculated() && !this->cudaUsed)
	{
		objectModel->CalculateKeyPointsAndDescriptors(this->detector, this->extractor); // Calculate keypoints from model
	}

	// Get keypoints and descriptors from model
	keypointsObject = objectModel->Keypoints();
	descriptorsObject = objectModel->Descriptors();

	// --------------------------------------------------
	// Scene and model preparation end
	// --------------------------------------------------

	// --------------------------------------------------
	// Feature matching algorithm
	// --------------------------------------------------
	// If object and scene descriptor and keypoints exists..
	if (!this->cudaUsed && !descriptorsObject.empty() && !descriptorsScene.empty() && !keypointsObject.empty() && !keypointsScene.empty())
	{
		// If matching type is flan based, scene and object must be in CV_32F format
		if (matcherType == cv::DescriptorMatcher::FLANNBASED)
		{
			descriptorsScene.convertTo(descriptorsScene, CV_32F);
			descriptorsObject.convertTo(descriptorsObject, CV_32F);
		}

		// ------ CPU USAGE ------
		// matching descriptor vectors
		matcher->knnMatch(descriptorsObject, descriptorsScene, matches, DEFAULT_NEIGHBOR);

		// Ratio test for good matches - http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
		// Neighbourhoods comparison
		RatioTest(matches, goodMatches, DEFAULT_RATIO_VALUE);

		drawable = ObtainMatchingResult(sceneImage,
			objectImage,
			goodMatches,
			keypointsObject,
			keypointsScene,
			sceneModel,
			objectModel,
			isIRAUsed,
			isROIUsed,
			roi);
	}
#if Companion_USE_CUDA
	else if (cudaUsed)
	{
		// ------ Cuda USAGE ------
		if (cv::cuda::getCudaEnabledDeviceCount() == 0)
		{
			throw Companion::Error::Code::no_cuda_device;
		}

		cv::cuda::GpuMat gpu_scene(sceneImage); // Load scene as an gpu mat
		cv::cuda::GpuMat gpu_object(objectImage); // Load object as an gpu mat
		cv::cuda::GpuMat gpu_descriptors_scene, gpu_descriptors_object;
		cv::Ptr<cv::cuda::DescriptorMatcher> gpu_matcher;

		if (cudaFeatureMatching != nullptr)
		{
			this->cudaFeatureMatching->detectAndCompute(gpu_scene, cv::noArray(), keypointsScene, gpu_descriptors_scene);
			this->cudaFeatureMatching->detectAndCompute(gpu_object, cv::noArray(), keypointsObject, gpu_descriptors_object);
			gpu_matcher = cv::cuda::DescriptorMatcher::createBFMatcher(this->cudaFeatureMatching->defaultNorm());
			gpu_matcher->knnMatch(gpu_descriptors_object, gpu_descriptors_scene, matches, 2);
		}
#if Companion_USE_XFEATURES_2D && Companion_USE_CUDA
		else
		{
			surf_cuda(gpu_scene, cv::cuda::GpuMat(), keypointsScene, gpu_descriptors_scene);
			surf_cuda(gpu_object, cv::cuda::GpuMat(), keypointsObject, gpu_descriptors_object);
			gpu_matcher = cv::cuda::DescriptorMatcher::createBFMatcher(surf_cuda.defaultNorm());
			gpu_matcher->knnMatch(gpu_descriptors_object, gpu_descriptors_scene, matches, 2);
		}
#endif

		gpu_scene.release();
		gpu_object.release();

		// ToDO := Default RATIO Value must be set as configuration value
		// ToDo := SURF_CUDA results are not good
		// Ratio test for good matches - http://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf#page=20
		// Neighborhoods comparison
		RatioTest(matches, goodMatches, DEFAULT_RATIO_VALUE);

		drawable = ObtainMatchingResult(sceneImage,
			objectImage,
			goodMatches,
			keypointsObject,
			keypointsScene,
			sceneModel,
			objectModel,
			isIRAUsed,
			isROIUsed,
			roi);
	}
#endif
	else
	{
		// If results are not good enough and empty for keypoints and descriptors
		sceneImage.release();
		objectImage.release();
		RepeatAlgorithm(sceneModel, objectModel, roi, isIRAUsed, ira, isROIUsed);
	}

	if (drawable != nullptr)
	{
		// TODO := SCORING CALCULATION
		// Object found
		result = std::make_shared<RESULT_RECOGNITION>(100, objectModel->ID(), drawable);

		sceneImage.release();
		objectImage.release();
	}
	else if (isIRAUsed || isROIUsed)
	{
		// No results and IRA or ROI was used
		sceneImage.release();
		objectImage.release();
		RepeatAlgorithm(sceneModel, objectModel, roi, isIRAUsed, ira, isROIUsed);
	}

	return result;
}

bool Companion::Algorithm::Recognition::Matching::FeatureMatching::IsCuda() const
{
	return this->cudaUsed;
}

void Companion::Algorithm::Recognition::Matching::FeatureMatching::CalculateKeyPoints(PTR_MODEL_FEATURE_MATCHING model)
{
	if (!IsCuda())
	{
		model->CalculateKeyPointsAndDescriptors(this->detector, this->extractor);
	}
}

PTR_RESULT_RECOGNITION Companion::Algorithm::Recognition::Matching::FeatureMatching::RepeatAlgorithm(
	PTR_MODEL_FEATURE_MATCHING sceneModel,
	PTR_MODEL_FEATURE_MATCHING objectModel,
	PTR_DRAW_FRAME roi,
	bool isIRAUsed,
	PTR_IMAGE_REDUCTION_ALGORITHM ira,
	bool isROIUsed)
{
	if (isIRAUsed)
	{
		ira->Clear(); // Clear last recognized object position
		return ExecuteAlgorithm(sceneModel, objectModel, roi); // Repeat algorithm and full scene or roi
	}
	else if (isROIUsed)
	{
		return ExecuteAlgorithm(sceneModel, objectModel, nullptr); // Repeat algorithm and check full scene
	}

	return nullptr;
}

void Companion::Algorithm::Recognition::Matching::FeatureMatching::RatioTest(const std::vector<std::vector<cv::DMatch>>& matches,
	std::vector<cv::DMatch>& good_matches,
	float ratio)
{
	unsigned long position = 0;
	bool insertElement;
	for (size_t i = 0; i < matches.size(); ++i)
	{
		if (matches[i].size() >= 2 && (matches[i][0].distance < ratio * matches[i][1].distance))
		{
			position = 0;
			insertElement = false;
			if (good_matches.empty()) {
				good_matches.push_back(matches[i][0]);
			}
			else {
				while (position < good_matches.size() && !insertElement) {
					if (good_matches.at(position).distance > matches[i][0].distance) {
						good_matches.insert(good_matches.begin() + position, matches[i][0]);
						insertElement = true;
						if (good_matches.size() > this->countMatches) {
							// Delete last element if maximum is reached
							good_matches.erase(good_matches.end() - 1);
						}
					}
					position++;
				}
				if (!insertElement && good_matches.size() < this->countMatches) {
					good_matches.push_back(matches[i][0]);
				}
			}

		}
	}

}

void Companion::Algorithm::Recognition::Matching::FeatureMatching::ObtainKeypointsFromGoodMatches(
	const std::vector<cv::DMatch>& good_matches,
	const std::vector<cv::KeyPoint>& keypoints_object,
	const std::vector<cv::KeyPoint>& keypoints_scene,
	std::vector<cv::Point2f>& feature_points_object,
	std::vector<cv::Point2f>& feature_points_scene) {

	int trainIdx;
	int queryIdx;

	// Get the keypoints from the good matches
	for (size_t i = 0; i < good_matches.size(); i++)
	{
		trainIdx = good_matches[i].trainIdx;
		queryIdx = good_matches[i].queryIdx;

		if ((trainIdx > 0 && keypoints_scene.size() > trainIdx) && (queryIdx > 0 && keypoints_object.size() > queryIdx))
		{
			feature_points_scene.push_back(keypoints_scene[trainIdx].pt);
			feature_points_object.push_back(keypoints_object[queryIdx].pt);
		}
	}

}

PTR_DRAW Companion::Algorithm::Recognition::Matching::FeatureMatching::ObtainMatchingResult(
	cv::Mat& sceneImage,
	cv::Mat& objectImage,
	std::vector<cv::DMatch>& good_matches,
	std::vector<cv::KeyPoint>& keypoints_object,
	std::vector<cv::KeyPoint>& keypoints_scene,
	PTR_MODEL_FEATURE_MATCHING sModel,
	PTR_MODEL_FEATURE_MATCHING cModel,
	bool isIRAUsed,
	bool isROIUsed,
	PTR_DRAW_FRAME roi)
{

	PTR_DRAW drawable = nullptr;
	cv::Mat homography;
	std::vector<cv::Point2f> feature_points_object, feature_points_scene;

	feature_points_object.clear();
	feature_points_scene.clear();

	// Count of good matches if results are good enough.
	if (good_matches.size() >= this->countMatches)
	{

		ObtainKeypointsFromGoodMatches(good_matches,
			keypoints_object,
			keypoints_scene,
			feature_points_object,
			feature_points_scene);

		// Find Homography if only features points are filled
		if (!feature_points_object.empty() && !feature_points_scene.empty())
		{
			homography = cv::findHomography(feature_points_object,
				feature_points_scene,
				this->findHomographyMethod,
				this->reprojThreshold,
				cv::noArray(),
				this->ransacMaxIters);

			if (!homography.empty())
			{
				drawable = CalculateArea(homography, sceneImage, objectImage, sModel, cModel, isIRAUsed, isROIUsed, roi);
			}
		}

	}

	return drawable;
}

PTR_DRAW Companion::Algorithm::Recognition::Matching::FeatureMatching::CalculateArea(
	cv::Mat& homography,
	cv::Mat& sceneImage,
	cv::Mat& objectImage,
	PTR_MODEL_FEATURE_MATCHING sModel,
	PTR_MODEL_FEATURE_MATCHING cModel,
	bool isIRAUsed,
	bool isROIUsed,
	PTR_DRAW_FRAME roi)
{

	PTR_DRAW_FRAME frame = nullptr;
	cv::Mat originalImg = sModel->Image();

	//-- Get the corners from the image_1 (the object to be recognized)
	std::vector<cv::Point2f> obj_corners(4);
	obj_corners[0] = cv::Point2f(0, 0);
	obj_corners[1] = cv::Point2f(objectImage.cols, 0);
	obj_corners[2] = cv::Point2f(objectImage.cols, objectImage.rows);
	obj_corners[3] = cv::Point2f(0, objectImage.rows);

	std::vector<cv::Point2f> scene_corners(4);
	cv::perspectiveTransform(obj_corners, scene_corners, homography);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	cv::Rect lastRect = cv::Rect();
	PTR_IMAGE_REDUCTION_ALGORITHM ira = cModel->Ira();

	if (isIRAUsed) // IRA was used
	{
		lastRect = ira->LastObjectPosition();
	}
	else if (isROIUsed)
	{
		lastRect = cv::Rect(roi->TopLeft(), roi->BottomRight());
	}

	// Offset is recalculate position from last recognition if exists
	cv::Point2f offset = cv::Point2f(lastRect.x, lastRect.y);

	// Focus area - Scene Corners
	//   0               1
	//   -----------------
	//   |               |
	//   |               |
	//   |               |
	//   |               |
	//   -----------------
	//   3               2

	float minX = sceneImage.cols;
	float maxX = 0.0f;
	float minY = sceneImage.rows;
	float maxY = 0.0f;

	for (cv::Point2f point : scene_corners)
	{
		if (point.x < minX) { minX = point.x; }
		if (point.x > maxX) { maxX = point.x; }
		if (point.y < minY) { minY = point.y; }
		if (point.y > maxY) { maxY = point.y; }
	}

	float width = maxX - minX;
	float height = maxY - minY;

	cv::Point2f scale = cv::Point2f(width / 2.0f, height / 2.0f);
	cv::Point2f start = cv::Point2f(minX, minY) + offset - scale;
	cv::Point2f end = cv::Point2f(maxX, maxY) + offset + scale;

	// Object area
	cv::Point2f topLeft = scene_corners[0] + offset;
	cv::Point2f topRight = scene_corners[1] + offset;
	cv::Point2f bottomRight = scene_corners[2] + offset;
	cv::Point2f bottomLeft = scene_corners[3] + offset;

	// Validate the rectangular shape of the recognized area
	if (Companion::Util::ValidateShape(topRight, bottomLeft, topLeft, bottomRight, this->minSidelLength))
	{
		// Create a drawable frame to represent the calculated area
		frame = std::make_shared<DRAW_FRAME>(topLeft, topRight, bottomLeft, bottomRight);
	}

	// If IRA is used...
	if (useIRA && frame != nullptr)
	{

		// IRA stores position from the recognized object
		ira->LastObjectPosition(start.x, start.y, end.x - start.x, end.y - start.y);
		const cv::Rect& lastObjectPosition = ira->LastObjectPosition();

		// Check if start point is set correctly
		if (lastObjectPosition.x < 0)
		{
			ira->X(0);
		}

		// Check if width is not oversized
		if (lastObjectPosition.width + lastObjectPosition.x > originalImg.cols)
		{
			ira->Width(originalImg.cols - lastObjectPosition.x);
		}

		if (lastObjectPosition.y < 0) {
			ira->Y(0);
		}

		if (lastObjectPosition.height + lastObjectPosition.y > originalImg.rows)
		{
			ira->Height(originalImg.rows - lastObjectPosition.y);
		}

		if (lastObjectPosition.area() <= 0)
		{
			// Something goes wrong in area clear IRA
			ira->Clear();
		}
	}

	return frame;
}

void Companion::Algorithm::Recognition::Matching::FeatureMatching::UseIRA(bool useIRA)
{
	this->useIRA = useIRA;
}


