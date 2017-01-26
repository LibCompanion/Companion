#include "ConsumerStream.h"

void ConsumerStream::run(CompanionConfig *config) {
    cv::Mat frame;
    ImageProcessing *processing = config->getProcessing();
    int frameNr = 0;
    bool finished = false;

    while (!finished) {
        while (queue.pop(frame)) {
            if(!frame.empty()) {
                if(config->getSkipFrame() == -1) {
                    // --> Send frame to image processing component.
                    processing->execute(frame);
                } else {
                    if(frameNr == config->getSkipFrame()) {
                        processing->execute(frame);
                        frameNr = 0;
                    }
                    frameNr++;
                }
                frame.release();
            } else {
                finished = true;
            }
        }
    }
}