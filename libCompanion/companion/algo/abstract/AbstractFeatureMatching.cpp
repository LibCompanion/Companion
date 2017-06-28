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

#include "AbstractFeatureMatching.h"

void Companion::Algorithm::AbstractFeatureMatching::ratio_test(const std::vector<std::vector<cv::DMatch>> &matches,
                                                               std::vector<cv::DMatch> &good_matches,
                                                               float ratio) {
    for (int i = 0; i < matches.size(); ++i) {
        if (matches[i].size() >= 2 && (matches[i][0].distance < ratio * matches[i][1].distance)) {
            good_matches.push_back(matches[i][0]);
        }
    }

}

void Companion::Algorithm::AbstractFeatureMatching::obtainKeypointsFromGoodMatches(
        std::vector<cv::DMatch> &good_matches,
        std::vector<cv::KeyPoint> &keypoints_object,
        std::vector<cv::KeyPoint> &keypoints_scene,
        std::vector<cv::Point2f> &feature_points_object,
        std::vector<cv::Point2f> &feature_points_scene) {

    int trainIdx;
    int queryIdx;

    // Get the keypoints from the good matches
    for (int i = 0; i < good_matches.size(); i++) {
        trainIdx = good_matches[i].trainIdx;
        queryIdx = good_matches[i].queryIdx;

        if ((trainIdx > 0 && keypoints_scene.size() > trainIdx) && (queryIdx > 0 && keypoints_object.size() > queryIdx)) {
            feature_points_scene.push_back(keypoints_scene[trainIdx].pt);
            feature_points_object.push_back(keypoints_object[queryIdx].pt);
        }
    }

}

Companion::Draw::Drawable* Companion::Algorithm::AbstractFeatureMatching::obtainMatchingResult(
        cv::Mat &sceneImage,
        cv::Mat &objectImage,
        std::vector<cv::DMatch> &good_matches,
        std::vector<cv::KeyPoint> &keypoints_object,
        std::vector<cv::KeyPoint> &keypoints_scene,
        Model::Processing::FeatureMatchingModel *sModel,
        Model::Processing::FeatureMatchingModel *cModel) {

    Companion::Draw::Drawable *lines = nullptr;
    cv::Mat homography;
    std::vector<cv::Point2f> feature_points_object, feature_points_scene;

    feature_points_object.clear();
    feature_points_scene.clear();

    // Count of good matches if results are good enough.
    if (good_matches.size() > countMatches) {

        obtainKeypointsFromGoodMatches(good_matches,
                                       keypoints_object,
                                       keypoints_scene,
                                       feature_points_object,
                                       feature_points_scene);

        // Find Homography if only features points are filled.
        if(!feature_points_object.empty() && !feature_points_scene.empty()) {
            homography = cv::findHomography(feature_points_object, feature_points_scene, CV_RANSAC);
            if (!homography.empty()) {
                lines = calculateArea(homography, sceneImage, objectImage, sModel, cModel);
            }
        }

    }

    return lines;
}

Companion::Draw::Drawable* Companion::Algorithm::AbstractFeatureMatching::calculateArea(
        cv::Mat &homography,
        cv::Mat &sceneImage,
        cv::Mat &objectImage,
        Model::Processing::FeatureMatchingModel *sModel,
        Model::Processing::FeatureMatchingModel *cModel) {

    Companion::Draw::Frame *frame = nullptr;

    //-- Get the corners from the image_1 (the object to be "detected")
    std::vector<cv::Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0, 0);
    obj_corners[1] = cvPoint(objectImage.cols, 0);
    obj_corners[2] = cvPoint(objectImage.cols, objectImage.rows);
    obj_corners[3] = cvPoint(0, objectImage.rows);

    std::vector<cv::Point2f> scene_corners(4);
    cv::perspectiveTransform(obj_corners, scene_corners, homography);

    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    cv::Rect lastRect;
    IRA *ira = cModel->getIra();

    // IRA
    if(useIRA) {
        lastRect = ira->getLastObjectPosition();
    } else {
        lastRect = cv::Rect();
    }

    // Offset is recalculate position from last recognition if exists.
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
    float maxX = 0;
    float minY = sceneImage.rows;
    float maxY = 0;

    for (cv::Point2f point : scene_corners) {
        if (point.x < minX) { minX = point.x; }
        if (point.x > maxX) { maxX = point.x; }
        if (point.y < minY) { minY = point.y; }
        if (point.y > maxY) { maxY = point.y; }
    }

    float width = maxX - minX;
    float height = maxY - minY;

    cv::Point2f scale = cv::Point2f(width / 2, height / 2);
    cv::Point2f start = cv::Point2f(minX, minY) + offset - scale;
    cv::Point2f end = cv::Point2f(maxX, maxY) + offset + scale;

    // If IRA was used...
    if(sceneImage.cols != sModel->getImage().cols || sceneImage.rows != sModel->getImage().rows) {
        // Restore to original image.
        sceneImage = sModel->getImage();
    }

    // Object area.

    cv::Point2f topLeft = scene_corners[0] + offset;
    cv::Point2f topRight = scene_corners[1] + offset;
    cv::Point2f bottomRight = scene_corners[2] + offset;
    cv::Point2f bottomLeft = scene_corners[3] + offset;

    // Check for minimum corner distance
    if (Companion::Util::hasDistantPosition(topLeft, topRight, cornerDistance) &&
        Companion::Util::hasDistantPosition(topLeft, bottomRight, cornerDistance) &&
        Companion::Util::hasDistantPosition(topLeft, bottomLeft, cornerDistance) &&
        Companion::Util::hasDistantPosition(topRight, bottomRight, cornerDistance) &&
        Companion::Util::hasDistantPosition(topRight, bottomLeft, cornerDistance) &&
        Companion::Util::hasDistantPosition(bottomRight, bottomLeft, cornerDistance)) {
        
        // Create a drawable frame to represent the calculated area
        frame = new Companion::Draw::Frame(topLeft, topRight, bottomLeft, bottomRight);
    }

    // If IRA is used...
    if(useIRA) {

        // IRA algo stores position from detected object.
        ira->setLastObjectPosition(start.x, start.y, end.x - start.x, end.y - start.y);
        const cv::Rect &lastObjectPosition = ira->getLastObjectPosition();

        // Check if start point is set correctly
        if (lastObjectPosition.x < 0) {
            ira->setX(0);
        }

        // Check if width is not oversized
        if (lastObjectPosition.width + lastObjectPosition.x > sceneImage.cols) {
            ira->setWidth(sceneImage.cols - lastObjectPosition.x);
        }

        if (lastObjectPosition.y < 0) {
            ira->setY(0);
        }

        if (lastObjectPosition.height + lastObjectPosition.y > sceneImage.rows) {
            ira->setHeight(sceneImage.rows - lastObjectPosition.y);
        }

        if (lastObjectPosition.area() <= 0) {
            // Something goes wrong in area clear IRA.
            ira->clear();
        }
    }

    return frame;
}