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

#include "CudaFeatureMatching.h"

Companion::Algorithm::Cuda::FeatureMatching::FeatureMatching(cv::Ptr<cv::Feature2D> cudaFeatureMatching) {
    this->cudaFeatureMatching = cudaFeatureMatching;

}

Companion::Algorithm::Cuda::FeatureMatching::~FeatureMatching() {

}

Companion::Draw::Drawable* Companion::Algorithm::Cuda::FeatureMatching::algo(
        Model::ImageRecognitionModel *searchModel,
        Model::ImageRecognitionModel *compareModel) {

    Companion::Draw::Drawable *lines = nullptr;
    int ngpus = cv::cuda::getCudaEnabledDeviceCount();

    cv::Mat img_matches;
    cv::Mat scene_rgb = searchModel->getImage();
    cv::Mat object = compareModel->getImage();
    cv::Mat scene;
    std::vector<std::vector<cv::DMatch>> matches;
    std::vector<cv::DMatch> good_matches;
    std::vector<cv::KeyPoint> keypoints_scene, keypoints_object;

    // Clear all lists from last run.
    keypoints_scene.clear();
    keypoints_object.clear();
    good_matches.clear();
    matches.clear();

    // ToDo := Check if Image is in correct format
    cvtColor(scene_rgb, scene, CV_RGB2GRAY);

    Companion::Model::FeatureMatchingModel *sModel = dynamic_cast<Companion::Model::FeatureMatchingModel *>(searchModel);
    Companion::Model::FeatureMatchingModel *cModel = dynamic_cast<Companion::Model::FeatureMatchingModel *>(compareModel);

    if(ngpus > 0) {

        // ---- Cuda start
        cv::cuda::GpuMat gpu_scene(scene);
        cv::cuda::GpuMat gpu_object(object);
        cv::cuda::GpuMat gpu_descriptors_scene, gpu_descriptors_object;

        cudaFeatureMatching->detectAndCompute(gpu_scene, cv::noArray(), keypoints_scene, gpu_descriptors_scene);
        cudaFeatureMatching->detectAndCompute(gpu_object, cv::noArray(), keypoints_object, gpu_descriptors_object);

        cv::Ptr<cv::cuda::DescriptorMatcher> gpu_matcher = cv::cuda::DescriptorMatcher::createBFMatcher(cudaFeatureMatching->defaultNorm());

        gpu_matcher->knnMatch(gpu_descriptors_object, gpu_descriptors_scene, matches, 2);

        gpu_scene.release();
        gpu_object.release();
        // ---- Cuda end

        ratio_test(matches, good_matches, 0.80);
        lines = obtainMatchingResult(scene, object, good_matches, keypoints_object, keypoints_scene, sModel, cModel);
    }

    return lines;
}

bool Companion::Algorithm::Cuda::FeatureMatching::isCuda() {
    return true;
}