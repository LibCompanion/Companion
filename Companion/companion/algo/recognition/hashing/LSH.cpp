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

#include "LSH.h"

Companion::Model::Result::RecognitionResult* Companion::Algorithm::Recognition::Hashing::LSH::executeAlgorithm(Companion::Model::Processing::ImageHashModel * model,
    cv::Mat query, 
    Companion::Draw::Frame *roi)
{
    Companion::Model::Result::RecognitionResult* result = nullptr;
    std::vector<std::pair<int, float>> scores = model->getScores();
    std::pair<cv::Mat_<float>, cv::Mat> dataset = model->generateDataset();
    cv::Mat_<float> hashImages = dataset.first;
    cv::Mat datasetImages = dataset.second;

    query.reshape(1, 1).convertTo(query, CV_32F);
    query = query * hashImages;

    for (size_t i = 0; i < query.rows; i++)
    {
        for (size_t j = 0; j < query.cols; j++)
        {
            query.at<float>(i, j) = query.at<float>(i, j) > 0 ? 1 : 0;
        }
    }

    query.convertTo(query, CV_8U);

    //Search for similar samples in the dataset
    for (size_t row = 0; row < datasetImages.rows; ++row)
    {
        scores[row].second = norm(query, datasetImages.row(row), cv::NORM_HAMMING);
    }

    //Make a copy of scores and rank them
    std::vector<std::pair<int, float>> rank;
    rank.assign(scores.begin(), scores.end());
    sort(rank.begin(), rank.end(), sortRank());

    for (size_t r = 0; r < std::min(10, (int)rank.size()); ++r)
    {
        if (r == 0)
        {
            result = new Companion::Model::Result::RecognitionResult(static_cast<int>(rank.at(r).second), rank.at(r).first, roi);
        }
    }

    return result;
}

bool Companion::Algorithm::Recognition::Hashing::LSH::isCuda() const
{
    return false;
}
