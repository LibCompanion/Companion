//
// Created by asekulsk on 13.12.16.
//

#include "ProducerStream.h"

void ProducerStream::run(std::string videoPath) {
    try {
        Video video;
        cv::Mat frame;
        video.playVideo(videoPath);
        // Camera api
        frame = video.obtainImage();

        while(!frame.empty()) {
            queue.add(frame);
            frame = video.obtainImage();
        }

    } catch (CompanionError::error_code error) {
        std::cout << Util::get_error(error);
    }
}