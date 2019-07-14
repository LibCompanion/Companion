/*
 * This program is an object recognition framework written with OpenCV.
 * Copyright (C) 2016-2018 Andreas Sekulski, Dimitri Kotlovsky
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

#include "Configuration.h"

Companion::Configuration::Configuration()
{
    this->source = nullptr;
    this->processing = nullptr;
    this->worker = nullptr;
    this->skipFrame = 0;
    this->threadsRunning = false;
    this->imageBuffer = 5;
}

void Companion::Configuration::Run()
{
    if (this->threadsRunning)
    {
        // Stop active worker if running
        this->Stop();
    }
    else
    {
        // Create a new worker thread for execution only if no threads are active
		this->worker = std::make_shared<STREAM_WORKER>(this->imageBuffer, this->colorFormat);

        // Get all configuration data
        // Throws Error if invalid settings are set.
		PTR_STREAM stream = this->Source();
        PTR_IMAGE_PROCESSING imageProcessing = this->Processing();
        int skipFrame = this->SkipFrame();
        std::function<ERROR_CALLBACK> errorCallback = this->ErrorCallback();
        std::function<SUCCESS_CALLBACK> successCallback = this->ResultCallback();

        // Run new worker class.
        this->threadsRunning = true;
        this->producer = std::thread(&Thread::StreamWorker::Produce, this->worker, stream, skipFrame, errorCallback);
        this->consumer = std::thread(&Thread::StreamWorker::Consume, this->worker, imageProcessing, errorCallback, successCallback);
        this->producer.join();
        this->consumer.join();
        this->threadsRunning = false;
    }
}

void Companion::Configuration::Stop()
{
    if (this->threadsRunning && (this->source != nullptr) && !this->source->IsFinished())
    {
        // To stop running worker threads stop streams.
        this->source->Finish();
        this->source = nullptr;
        this->worker = nullptr;
    }
}

PTR_STREAM Companion::Configuration::Source() const
{

    if (this->source == nullptr)
    {
        throw Error::Code::stream_src_not_set;
    }

    return this->source;
}

void Companion::Configuration::Source(PTR_STREAM source)
{
    this->source = source;
}

PTR_IMAGE_PROCESSING Companion::Configuration::Processing() const
{

    if (this->processing == nullptr)
    {
        throw Error::Code::no_image_processing_algo_set;
    }

    return this->processing;
}

void Companion::Configuration::Processing(PTR_IMAGE_PROCESSING processing)
{
    this->processing = processing;
}

int Companion::Configuration::SkipFrame() const
{
    return this->skipFrame;
}

void Companion::Configuration::SkipFrame(int skipFrame)
{

    if (skipFrame <= 0)
    {
        skipFrame = 0;
    }

    this->skipFrame = skipFrame;
}

int Companion::Configuration::ImageBuffer() const
{
    return this->imageBuffer;
}

void Companion::Configuration::ImageBuffer(int imageBuffer)
{

    if (imageBuffer <= 0)
    {
        imageBuffer = 5;
    }

    this->imageBuffer = imageBuffer;
}

void Companion::Configuration::ResultCallback(std::function<SUCCESS_CALLBACK> callback, Companion::ColorFormat colorFormat)
{
    this->callback = callback;
    this->colorFormat = colorFormat;
}

const std::function<SUCCESS_CALLBACK>& Companion::Configuration::ResultCallback() const
{

	if (!this->callback)
	{
		throw Error::Code::no_handler_set;
	}

	return this->callback;
}

void Companion::Configuration::ErrorCallback(std::function<ERROR_CALLBACK> callback)
{
    this->errorCallback = callback;
}

const std::function<ERROR_CALLBACK> &Companion::Configuration::ErrorCallback() const
{

    if (!this->errorCallback)
    {
        throw Error::Code::no_handler_set;
    }

    return this->errorCallback;
}
