#include "ConsumerStream.h"

void ConsumerStream::run(CompanionConfig *config) {
    cv::Mat frame;
    ImageProcessing *processing = config->getProcessing();

    // ToDo Stop function
    while (true) {
        while (queue.pop(frame)) {
            // --> Send frame to image processing component.
            processing->execute(frame);
            frame.release();
        }
    }
}