#ifndef COMPANION_VIDEO_CAPTURE_H
#define COMPANION_VIDEO_CAPTURE_H

#include <opencv2/opencv.hpp>

/**
 * Video streaming OpenCV realization to obtain images from an video or livestream like an camera.
 *
 * @author Andreas Sekulski
 */
class Video {

public:

    /**
     * Default constructor
     */
    Video();

    /**
     * Default destructor.
     */
    ~Video();

    /**
     * Connects to an given device number.
     * @param device Device number to connect.
     * @return 0 if connection successfully otherwise an negative value for an error.
     */
    int connectToDevice(int device);

    /**
     * Plays an video stream from given url.
     * @param url Video path to stream.
     * @return 0 if connection successfully otherwise an negative value for an error.
     */
    int playVideo(std::string url);

    /**
     * Obtain next image from open video stream.
     * @return An empty cv::Mat object if no image is obtained otherwise an cv::Mat entity from image.
     */
    cv::Mat obtainImage();

private:

    /**
     * Stores video device or video streaming.
     */
    cv::VideoCapture capture;
};

#endif //COMPANION_VIDEO_CAPTURE_H