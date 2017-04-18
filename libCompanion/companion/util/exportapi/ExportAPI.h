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

#ifndef COMPANION_EXPORT_API_H
#define COMPANION_EXPORT_API_H

#include "companion/Companion.h"
#include "companion/util/exportapi/ExportAPIDefinitions.h"

COMPAPI(Companion*) CreateCompanion();
COMPAPI(void) DisposeCompanion(Companion* pObject);
COMPAPI(void) CallRun(Companion* pObject, StreamWorker &worker);
COMPAPI(void) CallStop(Companion* pObject);






/*
Stream *getSource() const;
param source Video source to set like an camera or video.
void setSource(Stream *source);
bool addModel(ImageRecognitionModel *model);
void removeModel(ImageRecognitionModel *model);
void clearModels();
const std::vector<ImageRecognitionModel *> &getModels() const;
ImageProcessing *getProcessing() const;
void setProcessing(ImageProcessing *processing);
int getSkipFrame() const;
void setSkipFrame(int skipFrame);
void setResultHandler(std::function<SUCCESS_CALLBACK> callback);
const std::function<SUCCESS_CALLBACK> &getCallback() const;
void setErrorHandler(std::function<ERROR_CALLBACK> callback);
const std::function<ERROR_CALLBACK> &getErrorCallback() const;
*/

#endif //COMPANION_EXPORT_API_H