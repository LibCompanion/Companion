#include "AbstractFeatureMatching.h"

void AbstractFeatureMatching::ratio_test(const std::vector<std::vector<cv::DMatch>> &matches,
        std::vector<cv::DMatch> &good_matches,
        float ratio) {

    for (int i = 0; i < matches.size(); ++i) {
        if (matches[i][0].distance < ratio * matches[i][1].distance) {
            good_matches.push_back(matches[i][0]);
        }
    }

}

void AbstractFeatureMatching::symmetry_test(const std::vector<cv::DMatch> &matches1,
                                            const std::vector<cv::DMatch> &matches2,
                                            std::vector<cv::DMatch> &symMatches) {

    symMatches.clear();

    for (std::vector<cv::DMatch>::const_iterator matchIterator1 = matches1.begin();
         matchIterator1 != matches1.end(); ++matchIterator1) {

        for (std::vector<cv::DMatch>::const_iterator matchIterator2 = matches2.begin();
             matchIterator2 != matches2.end(); ++matchIterator2) {

            if ((*matchIterator1).queryIdx == (*matchIterator2).trainIdx &&
                (*matchIterator2).queryIdx == (*matchIterator1).trainIdx) {
                symMatches.push_back(
                        cv::DMatch((*matchIterator1).queryIdx, (*matchIterator1).trainIdx, (*matchIterator1).distance));
                break;
            }

        }

    }
}

void AbstractFeatureMatching::obtainKeypointsFromGoodMatches(std::vector<cv::DMatch> &good_matches,
                                                             std::vector<cv::KeyPoint> &keypoints_object,
                                                             std::vector<cv::KeyPoint> &keypoints_scene,
                                                             std::vector<cv::Point2f> &feature_points_object,
                                                             std::vector<cv::Point2f> &feature_points_scene) {
    // Get the keypoints from the good matches
    for (int i = 0; i < good_matches.size(); i++) {
        // ToDo := Segmentation fault if video ends... why... :(
        // Keypoints object is smaller then scene
        feature_points_scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
        feature_points_object.push_back(keypoints_object[good_matches[i].queryIdx].pt);
    }
}

Drawable* AbstractFeatureMatching::obtainMatchingResult(cv::Mat &sceneImage,
                                                        cv::Mat &objectImage,
                                                        std::vector<cv::DMatch> &good_matches,
                                                        std::vector<cv::KeyPoint> &keypoints_object,
                                                        std::vector<cv::KeyPoint> &keypoints_scene,
                                                        FeatureMatchingModel *sModel,
                                                        FeatureMatchingModel *cModel) {

    Drawable *lines = nullptr;
    cv::Mat homography;
    std::vector<cv::Point2f> feature_points_object, feature_points_scene;

    // Count of good matches if results are good enough.
    if (good_matches.size() > countMatches) {

        obtainKeypointsFromGoodMatches(good_matches,
                                       keypoints_object,
                                       keypoints_scene,
                                       feature_points_object,
                                       feature_points_scene);

        // Find Homography
        homography = cv::findHomography(feature_points_object, feature_points_scene, CV_RANSAC);

        if (!homography.empty()) {
            lines = calculateArea(homography, sceneImage, objectImage, sModel, cModel);
        }
    }

    return lines;
}

Drawable* AbstractFeatureMatching::calculateArea(cv::Mat &homography,
                                                 cv::Mat &sceneImage,
                                                 cv::Mat &objectImage,
                                                 FeatureMatchingModel *sModel,
                                                 FeatureMatchingModel *cModel) {

    //-- Get the corners from the image_1 (the object to be "detected")
    std::vector<cv::Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0, 0);
    obj_corners[1] = cvPoint(objectImage.cols, 0);
    obj_corners[2] = cvPoint(objectImage.cols, objectImage.rows);
    obj_corners[3] = cvPoint(0, objectImage.rows);

    std::vector<cv::Point2f> scene_corners(4);
    cv::perspectiveTransform(obj_corners, scene_corners, homography);

    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    int thickness = 4;
    cv::Scalar color = cv::Scalar(0, 255, 0);

    // IRA
    cv::Rect lastRect = cModel->getLastPosition();
    cv::Point2f offset = cv::Point2f(lastRect.x,
                                     lastRect.y); // Offset is recalculate position from last recognition

    // Focus area - Scene Corners
    //   0               1
    //   -----------------
    //   |               |
    //   |               |
    //   |               |
    //   |               |
    //   -----------------
    //   3               2
    float width = (scene_corners[1] - scene_corners[0]).x;
    float height = (scene_corners[3] - scene_corners[0]).y;
    cv::Point2f scale = cv::Point2f(width / 2, height / 2);
    cv::Point2f start = scene_corners[0] + offset - scale;
    cv::Point2f end = scene_corners[2] + offset + scale;

    // If IRA was used...
    if(sceneImage.cols != sModel->getImage().cols || sceneImage.rows != sModel->getImage().rows) {
        // Restore to original image.
        sceneImage = sModel->getImage();
    }

    // Object area.
    Lines *lines = new Lines();
    lines->addLine(new Line(scene_corners[0] + offset, scene_corners[1] + offset, color, thickness));
    lines->addLine(new Line(scene_corners[3] + offset, scene_corners[0] + offset, color, thickness));
    lines->addLine(new Line(scene_corners[1] + offset, scene_corners[2] + offset, color, thickness));
    lines->addLine(new Line(scene_corners[2] + offset, scene_corners[3] + offset, color, thickness));

    // IRA algo stores position from detected object with an scaling factor.
    cModel->setLastPosition(start.x, start.y, end.x - start.x, end.y - start.y);

    // Check if start point is set correctly
    if (cModel->getLastPosition().x < 0) {
        cModel->setLastPositionX(0);
    }

    // Check if width is not oversized
    if (cModel->getLastPosition().width + cModel->getLastPosition().x > sceneImage.cols) {
        cModel->setLastPositionWidth(sceneImage.cols - cModel->getLastPosition().x);
    }

    if (cModel->getLastPosition().y < 0) {
        cModel->setLastPositionY(0);
    }

    if (cModel->getLastPosition().height + cModel->getLastPosition().y > sceneImage.rows) {
        cModel->setLastPositionHeight(sceneImage.rows - cModel->getLastPosition().y);
    }

    if(cModel->getLastPosition().area() <= 0) {
        // Something goes wrong in area
        cModel->setLastPosition(-1, -1, 0, 0);
    }

    return lines;
}