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

#ifndef COMPANION_COMPANIONCONFIG_H
#define COMPANION_COMPANIONCONFIG_H

#include <companion/openCV3/stream/Video.h>
#include <companion/openCV3/model/ImageRecognitionModel.h>
#include <companion/openCV3/algo/ImageRecognition.h>
#include <companion/openCV3/processing/ImageProcessing.h>

class CompanionConfig {

public:

    CompanionConfig();

    virtual ~CompanionConfig();

    Video *getSource() const;

    void setSource(Video *source);

    void addModel(ImageRecognitionModel *model);

    void removeModel(ImageRecognitionModel *model);

    void clearModels();

    const std::vector<ImageRecognitionModel *> &getModels() const;

    ImageProcessing *getProcessing() const;

    void setProcessing(ImageProcessing *processing);

private:

    /**
     * Video source to obtain images from an device.
     */
    Video *source;

    /**
     * Search models to detect objects from source.
     */
    std::vector<ImageRecognitionModel*> models;

    ImageProcessing *processing;

};


#endif //COMPANION_COMPANIONCONFIG_H
