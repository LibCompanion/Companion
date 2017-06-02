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

#include "StreamWorker.h"

void Companion::Thread::StreamWorker::produce(Companion::Input::Stream *stream,
                                              int skipFrame,
                                              std::function<ERROR_CALLBACK> errorCallback) {

    int skipFrameNr = 0;

    try {

        cv::Mat frame = stream->obtainImage();

        while (!finished) {

            if(!frame.empty()) {
                // If skip frame is not used...
                if(skipFrame <= 0 && storeFrame(frame)) {
                    // obtain next frame to store.
                    frame = stream->obtainImage();
                } else if(skipFrame > 0) {
                    // ... check if skip frame nr is reached and If frame was stored
                    if(skipFrameNr == skipFrame && storeFrame(frame)) {
                        // obtain next frame to store.
                        frame = stream->obtainImage();
                        skipFrameNr = 0;
                    } else if(skipFrameNr != skipFrame) {
                        frame.release();
                        frame = stream->obtainImage();
                        skipFrameNr++;
                    }
                }
            } else {
                // If frames are empty loop.
                frame = stream->obtainImage();
            }
        }

        std::lock_guard<std::mutex> lk(mx);
        finished = true;
        cv.notify_all();

    } catch (Companion::Error::Code error) {
        errorCallback(error);
    }
}

void Companion::Thread::StreamWorker::consume(
        Companion::Processing::ImageProcessing *processing,
        std::function<ERROR_CALLBACK> errorCallback,
        std::function<SUCCESS_CALLBACK> callback) {

    cv::Mat frame;
    bool isFinished = false;

    try {
        while (!isFinished) {

            std::unique_lock<std::mutex> lk(mx);
            cv.wait(lk, [this]{return finished || !queue.empty();});

            if(!queue.empty()) {
                frame = queue.front();
                queue.pop();
                callback(processing->execute(frame), frame);
                frame.release();
            }

            if(finished) {
                isFinished = finished;
            }
        }
    } catch (Companion::Error::Code errorCode) {
        errorCallback(errorCode);
    }
}

bool Companion::Thread::StreamWorker::storeFrame(cv::Mat frame) {
    std::lock_guard<std::mutex> lk(mx);
    if(queue.size() >= buffer) {
        // If buffer full try to notify producer and wait current frame and do nothing.
        cv.notify_one();
        return false;
    } else {
        queue.push(frame);
        cv.notify_one();
        return true;
    }
}

bool Companion::Thread::StreamWorker::isRunning() {
    return !finished;
}

void Companion::Thread::StreamWorker::stop() {
    if(isRunning()) {
        finished = true;
    }
}