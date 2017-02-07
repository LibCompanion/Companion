#include "ConsumerStream.h"

void ConsumerStream::run(Companion *companion) {

    cv::Mat frame;
    ImageProcessing *processing;
    int frameNr = 0;
    bool finished = false;

    try {
        processing = companion->getProcessing();
    } catch (CompanionError::errorCode errorCode) {
        companion->executeError(errorCode);
    }

    while (!finished) {
        try {
            while (queue.pop(frame)) {

                if(!frame.empty()) {

                    if(companion->getSkipFrame() == -1) {
                        // --> Send frame to image processing component and send result to callback handler.
                        companion->executeResultHandler(processing->execute(frame), frame);
                    } else {
                        if(frameNr == companion->getSkipFrame()) {
                            // --> Send frame to image processing component and send result to callback handler.
                            companion->executeResultHandler(processing->execute(frame), frame);
                            frameNr = 0;
                        }
                        frameNr++;
                    }

                } else {
                    finished = true;
                }
            }
        } catch (CompanionError::errorCode errorCode) {
            companion->executeError(errorCode);
        }
    }
}