//
// Created by asekulsk on 13.12.16.
//

#ifndef COMPANION_CONSUMERSTREAM_H
#define COMPANION_CONSUMERSTREAM_H

#include <queue>
#include <opencv2/core.hpp>
#include <companion/openCV3/search/ImageRecognition.h>
#include <companion/openCV3/search/FeatureMatching.h>
#include <companion/openCV3/thread/wqueue.h>

class ConsumerStream {

public:

    ConsumerStream(wqueue<cv::Mat>& queue) : queue(queue) {}

    void run(std::string imgPath);

private:

    wqueue<cv::Mat>& queue;
};


#endif //COMPANION_CONSUMERSTREAM_H
