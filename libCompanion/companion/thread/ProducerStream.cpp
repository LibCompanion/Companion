#include "ProducerStream.h"

void ProducerStream::run(Companion *companion) {
    try {

        Video *video = companion->getSource();
        cv::Mat frame = video->obtainImage();

        while (!frame.empty()) {
			while (!queue.push(frame)) {
                // If buffer full wait and loop current frame and do nothing.
            }

            frame = video->obtainImage();
        }

        queue.push(frame);

    } catch (CompanionError::errorCode error) {
        companion->executeError(error);
    }
}