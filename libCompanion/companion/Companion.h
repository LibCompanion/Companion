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

#ifndef COMPANION_COMPANION_H
#define COMPANION_COMPANION_H

#include <functional>

#include "companion/stream/Video.h"
#include "companion/model/ImageRecognitionModel.h"
#include "companion/algo/ImageRecognition.h"
#include "companion/processing/ImageProcessing.h"

/**
 * Companion class to setup an image processing search.
 *
 * @author Andreas Sekulski
 */
class Companion {

public:

    Companion();

    virtual ~Companion();

    Video *getSource() const;

    void setSource(Video *source);

    void addModel(ImageRecognitionModel *model);

    void removeModel(ImageRecognitionModel *model);

    void clearModels();

    const std::vector<ImageRecognitionModel *> &getModels() const;

    ImageProcessing *getProcessing() const;

    void setProcessing(ImageProcessing *processing);

    int getSkipFrame() const;

    void setSkipFrame(int skipFrame);

    void setResultHandler(std::function<void(std::vector<Drawable*>, cv::Mat)> callback);

    void executeResultHandler(std::vector<Drawable*> objects, cv::Mat frame);

    void setErrorHandler(std::function<void(CompanionError::errorCode)> callback);

    void executeError(CompanionError::errorCode code);

private:

    /**
     * Callback event handler to send results back to main application.
     */
    std::function<void(std::vector<Drawable*>, cv::Mat)> callback;

    /**
     * Callback for an error.
     */
    std::function<void(CompanionError::errorCode)> errorCallback;

    /**
     * Video source to obtain images from an device.
     */
    Video *source;

    /**
     * Search models to detect objects from source.
     */
    std::vector<ImageRecognitionModel*> models;

    /**
     * Image processing implementation for example an object detection.
     */
    ImageProcessing *processing;

    /**
     * Number of frames to skip to process next image.
     */
    int skipFrame;

};

#endif //COMPANION_COMPANION_H