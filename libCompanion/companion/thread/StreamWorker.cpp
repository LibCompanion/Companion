#include "StreamWorker.h"

void StreamWorker::produce(Companion *companion) {

    bool storedFrame;
    int skipFrameNr = 0;

    try {

        Video *video = companion->getSource();
        cv::Mat frame = video->obtainImage();

        while (!frame.empty()) {

            if(companion->getSkipFrame() > 0) {
                storedFrame = storeFrame(frame);
            } else {
                if(skipFrameNr == companion->getSkipFrame()) {
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
                frame = video->obtainImage();
            }
        }

        std::lock_guard<std::mutex> lk(mx);
        finished = true;
        cv.notify_all();

    } catch (CompanionError::errorCode error) {
        companion->executeError(error);
    }
}

void StreamWorker::consume(Companion *companion) {

    cv::Mat frame;
    ImageProcessing *processing;
    bool isFinished = true;

    try {
        processing = companion->getProcessing();

        while (isFinished) {

            std::unique_lock<std::mutex> lk(mx);
            cv.wait(lk, [this]{return finished || !queue.empty();});
            frame = queue.front();
            queue.pop();
            companion->executeResultHandler(processing->execute(frame), frame);

            if(finished) {
                isFinished = finished;
            }
        }

    } catch (CompanionError::errorCode errorCode) {
        companion->executeError(errorCode);
    }
}

bool StreamWorker::storeFrame(cv::Mat &frame) {
    std::lock_guard<std::mutex> lk(mx);
    if(queue.size() >= buffer) {
        // If buffer full wait and loop current frame and do nothing.
        cv.notify_one();
        return false;
    } else {
        queue.push(frame);
        cv.notify_one();
        return true;
    }
}