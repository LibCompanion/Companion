/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2018 Andreas Sekulski
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StreamWorker.h"

Companion::Thread::StreamWorker::StreamWorker(int buffer, Companion::ColorFormat colorFormat)
{
    this->finished = false;
    this->colorFormat = colorFormat;
    this->buffer = buffer;
    if (this->buffer <= 0)
    {
        this->buffer = 1;
    }
}

void Companion::Thread::StreamWorker::produce(Companion::Input::Stream *stream,
    int skipFrame,
    std::function<ERROR_CALLBACK> errorCallback)
{

    int skipFrameNr = 0;
    cv::Mat frame;

    try
    {
        frame = stream->obtainImage();

        while (!stream->isFinished())
        {

            if (!frame.empty())
            {
                // If skip frame is not used...
                if (skipFrame <= 0 && storeFrame(frame))
                {
                    // obtain next frame to store.
                    frame = stream->obtainImage();
                }
                else if (skipFrame > 0)
                {
                    // ... check if skip frame nr is reached and If frame was stored
                    if (skipFrameNr == skipFrame && storeFrame(frame))
                    {
                        // obtain next frame to store.
                        frame = stream->obtainImage();
                        skipFrameNr = 0;
                    }
                    else if (skipFrameNr != skipFrame)
                    {
                        frame.release();
                        frame = stream->obtainImage();
                        skipFrameNr++;
                    }
                }
            }
            else
            {
                // If frames are empty loop.
                frame = stream->obtainImage();
            }
        }

        std::lock_guard<std::mutex> lk(this->mx);
        this->finished = true;
        this->cv.notify_all();

    }
    catch (Companion::Error::Code error)
    {
        errorCallback(error);
    }
}

void Companion::Thread::StreamWorker::consume(
    Companion::Processing::ImageProcessing *processing,
    std::function<ERROR_CALLBACK> errorCallback,
    std::function<SUCCESS_CALLBACK> successCallback)
{

    cv::Mat frame;
    cv::Mat resultBGR;

    while (!this->finished)
    {

        std::unique_lock<std::mutex> lk(this->mx);
        this->cv.wait(lk, [this] {return this->finished || !this->queue.empty(); });

        if (!this->queue.empty())
        {
            try
            {
                frame = this->queue.front();
                this->queue.pop();
                Companion::Util::convertColor(frame, resultBGR, this->colorFormat);
                successCallback(processing->execute(frame), resultBGR);
            }
            catch (Companion::Error::Code errorCode)
            {
                // Single error messages from processing
                errorCallback(errorCode);
            }
            catch (Companion::Error::CompanionException ex)
            {
                // Multiple error messages only called by parallelized methods.
                while (ex.hasNext())
                {
                    errorCallback(ex.next());
                }
            }

            frame.release();
            resultBGR.release();
        }

    }
    
}

bool Companion::Thread::StreamWorker::storeFrame(cv::Mat frame)
{
    std::lock_guard<std::mutex> lk(this->mx);
    if (this->queue.size() >= this->buffer)
    {
        // If buffer full try to notify producer and wait current frame and do nothing.
        this->cv.notify_one();
        return false;
    }
    else
    {
        this->queue.push(frame);
        this->cv.notify_one();
        return true;
    }
}
