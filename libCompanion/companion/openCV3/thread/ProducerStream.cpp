#include "ProducerStream.h"

void ProducerStream::run(std::string videoPath) {
    try {
        Video video;
        cv::Mat frame;
        video.playVideo(videoPath);
        //video.connectToDevice(0);

        frame = video.obtainImage(); // Camera api

        while (!frame.empty()) {
			while (!queue.push(frame)) {
                // If buffer full wait and loop current frame and do nothing.
            }
            frame = video.obtainImage();
        }
    } catch (CompanionError::error_code error) {
        std::cout << Util::get_error(error);
    }
}