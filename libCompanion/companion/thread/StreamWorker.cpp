#include "StreamWorker.h"

void Companion::Thread::StreamWorker::produce(Companion::Input::Stream *stream,
                                              int skipFrame,
                                              std::function<ERROR_CALLBACK> errorCallback) {

    bool storedFrame;
    int skipFrameNr = 0;

    try {

        cv::Mat frame = stream->obtainImage();

        while (!frame.empty() && !finished) {

            // If skip frame is not used...
            if(skipFrame > 0) {
                storedFrame = storeFrame(frame);
            } else {
                // ... check if skip frame nr is reached
                if(skipFrameNr == skipFrame) {
                    storedFrame = storeFrame(frame);
                    skipFrameNr = 0;
                } else {
                    // Indicator to obtain next frame.
                    storedFrame = true;
                    skipFrameNr++;
                }
            }

            // If frame was stored or skipped...
            if(storedFrame) {
                // obtain next frame to store.
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
            }

            if(finished) {
                isFinished = finished;
            }
        }
    } catch (Companion::Error::Code errorCode) {
        errorCallback(errorCode);
    }
}

bool Companion::Thread::StreamWorker::storeFrame(cv::Mat &frame) {
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