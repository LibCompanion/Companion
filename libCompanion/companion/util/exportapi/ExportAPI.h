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

//#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "companion/Companion.h"
#include "companion/processing/2D/ObjectDetection.h"
#include "companion/algo/cpu/CPUFeatureMatching.h"
#include "companion/stream/Video.h"
#include "companion/stream/Image.h"
#include "companion/util/exportapi/ExportAPIDefinitions.h"

/*
 * Wrapper functions for the export API (Windows DLL).
 */

/*********************/
/* Companion Wrapper */
/*********************/
COMPAPI(Companion*) createCompanion();
COMPAPI(void) disposeCompanion(Companion* companion);
COMPAPI(void) callRun(Companion* companion, StreamWorker &worker);
COMPAPI(void) callStop(Companion* companion);
COMPAPI(Stream*) callGetSource(Companion* companion); // const;
COMPAPI(void) callSetSource(Companion* companion, Stream* source);
COMPAPI(void) callAddModel(Companion* companion, ImageRecognitionModel* model);
COMPAPI(void) callRemoveModel(Companion* companion, ImageRecognitionModel* model);
COMPAPI(void) callClearModels(Companion* companion);
//const std::vector<ImageRecognitionModel *> &getModels(); // const;
COMPAPI(ImageProcessing*) callGetProcessing(Companion* companion); // const;
COMPAPI(void) callSetProcessing(Companion* companion, ImageProcessing* processing);
COMPAPI(int) callGetSkipFrame(Companion* companion); // const;
COMPAPI(void) callSetSkipFrame(Companion* companion, int skipFrame);
COMPAPI(void) callSetResultHandler(Companion* companion, std::function<SUCCESS_CALLBACK> callback);
//const std::function<SUCCESS_CALLBACK> &getCallback(); // const;
COMPAPI(void) callSetErrorHandler(Companion* companion, std::function<ERROR_CALLBACK> callback);
//const std::function<ERROR_CALLBACK> &getErrorCallback(); // const;

COMPAPI(Companion*) createCompanionFake();
void callback(std::vector<Drawable*> objects, cv::Mat frame);
void error(CompanionError::errorCode code);



/************************/
/* StreamWorker Wrapper */
/************************/
COMPAPI(StreamWorker*) createStreamWorker(std::queue<cv::Mat> &queue, int buffer = 1);
COMPAPI(void) disposeStreamWorker(StreamWorker* worker);

COMPAPI(StreamWorker*) createStreamWorkerFake(std::queue<cv::Mat>* queue, int buffer = 1);


/********************/
/* Helper Functions */
/********************/
COMPAPI(std::queue<cv::Mat>*) createQueue();
COMPAPI(void) disposeQueue(std::queue<cv::Mat>* queue);

#endif //COMPANION_EXPORT_API_H