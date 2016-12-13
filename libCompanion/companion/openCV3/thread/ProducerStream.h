//
// Created by asekulsk on 13.12.16.
//

#ifndef COMPANION_PRODUCERSTREAM_H
#define COMPANION_PRODUCERSTREAM_H

#include <queue>
#include <opencv2/core.hpp>
#include <companion/openCV3/stream/Video.h>
#include <companion/openCV3/thread/wqueue.h>

class ProducerStream {

public:
    ProducerStream(wqueue<cv::Mat> &queue);

    void run(std::string videoPath);

private:
    wqueue<cv::Mat> queue;
};


#endif //COMPANION_PRODUCERSTREAM_H
