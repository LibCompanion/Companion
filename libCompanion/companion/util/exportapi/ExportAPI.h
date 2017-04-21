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
#include "companion/input/Video.h"
#include "companion/input/Image.h"
#include "companion/util/exportapi/ExportAPIDefinitions.h"

namespace Companion {

    namespace ExportAPI {

        /*
         * Wrapper functions for the export API (Windows DLL).
         */

        /*************************/
        /* Configuration Wrapper */
        /*************************/
        COMPAPI(Companion::Configuration*) createConfiguration();
        COMPAPI(void) disposeConfiguration(Companion::Configuration* config);
        COMPAPI(void) callRun(Companion::Configuration* config, Companion::Thread::StreamWorker &worker);
        COMPAPI(void) callStop(Companion::Configuration* config);
        COMPAPI(Companion::Input::Stream*) callGetSource(Companion::Configuration* config); // const;
        COMPAPI(void) callSetSource(Companion::Configuration* config, Companion::Input::Stream* source);
        COMPAPI(void) callAddModel(Companion::Configuration* config, Companion::Model::ImageRecognitionModel* model);
        COMPAPI(void) callRemoveModel(Companion::Configuration* config, Companion::Model::ImageRecognitionModel* model);
        COMPAPI(void) callClearModels(Companion::Configuration* config);
        //const std::vector<ImageRecognitionModel *> &getModels(); // const;
        COMPAPI(Companion::Processing::ImageProcessing*) callGetProcessing(Companion::Configuration* config); // const;
        COMPAPI(void) callSetProcessing(Companion::Configuration* config, Companion::Processing::ImageProcessing* processing);
        COMPAPI(int) callGetSkipFrame(Companion::Configuration* config); // const;
        COMPAPI(void) callSetSkipFrame(Companion::Configuration* config, int skipFrame);
        COMPAPI(void) callSetResultHandler(Companion::Configuration* config, std::function<SUCCESS_CALLBACK> callback);
        //const std::function<SUCCESS_CALLBACK> &getCallback(); // const;
        COMPAPI(void) callSetErrorHandler(Companion::Configuration* config, std::function<ERROR_CALLBACK> callback);
        //const std::function<ERROR_CALLBACK> &getErrorCallback(); // const;

        COMPAPI(Companion::Configuration*) createCompanionFake();
        void callback(std::vector<Companion::Draw::Drawable*> objects, cv::Mat frame);
        void error(Companion::Error::Code code);



        /************************/
        /* StreamWorker Wrapper */
        /************************/
        COMPAPI(Companion::Thread::StreamWorker*) createStreamWorker(std::queue<cv::Mat> &queue, int buffer = 1);
        COMPAPI(void) disposeStreamWorker(Companion::Thread::StreamWorker* worker);

        COMPAPI(Companion::Thread::StreamWorker*) createStreamWorkerFake(std::queue<cv::Mat>* queue, int buffer = 1);


        /********************/
        /* Helper Functions */
        /********************/
        COMPAPI(std::queue<cv::Mat>*) createQueue();
        COMPAPI(void) disposeQueue(std::queue<cv::Mat>* queue);

    }
}

#endif //COMPANION_EXPORT_API_H