#include "ProducerStream.h"

void ProducerStream::run(CompanionConfig *config) {

    // ToDo Stop function
    try {
        Video *video = config->getSource();
        // ToDo Error Handling if video not set!

        cv::Mat frame = video->obtainImage();
        while (!frame.empty()) {
			while (!queue.push(frame)) {
                // If buffer full wait and loop current frame and do nothing.
            }
            frame = video->obtainImage();
        }
    } catch (CompanionError::errorCode error) {
        // ToDo := Error handling
        std::cout << CompanionError::getError(error);
    }
}