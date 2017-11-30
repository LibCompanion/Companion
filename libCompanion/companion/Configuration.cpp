/*
 * This program is an image recognition library written with OpenCV.
 * Copyright (C) 2016-2017 Andreas Sekulski
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

Companion::Configuration::~Configuration()
{
	delete this->source;
	delete this->processing;
	delete this->worker;
}

void Companion::Configuration::run()
{
	if (this->threadsRunning)
	{
		// Stop active worker if running
		this->stop();
	}
	else
	{
		// Create new worker for execution only if no threads are active
		this->worker = new Companion::Thread::StreamWorker(this->imageBuffer, this->colorFormat);

		// Get all configuration data
		// Throws Error if invalid settings are set.
		Companion::Input::Stream* stream = this->getSource();
		Companion::Processing::ImageProcessing* imageProcessing = this->getProcessing();
		int skipFrame = this->getSkipFrame();
		std::function<ERROR_CALLBACK> errorCallback = this->getErrorCallback();
		std::function<SUCCESS_CALLBACK> successCallback = this->getCallback();

		// Run new worker class.
		this->threadsRunning = true;
		this->producer = std::thread(&Thread::StreamWorker::produce, this->worker, stream, skipFrame, errorCallback);
		this->consumer = std::thread(&Thread::StreamWorker::consume, this->worker, imageProcessing, errorCallback, successCallback);
		this->producer.join();
		this->consumer.join();
		this->threadsRunning = false;
	}
}

void Companion::Configuration::stop()
{
	if (this->threadsRunning && this->source != nullptr && !this->source->isFinished())
	{
		// To stop running worker threads stop streams.
		this->source->finish();
		this->source = nullptr;
		this->worker = nullptr;
	}
}

Companion::Input::Stream *Companion::Configuration::getSource() const
{

	if (this->source == nullptr)
	{
		throw Error::Code::stream_src_not_set;
	}

	return this->source;
}

void Companion::Configuration::setSource(Companion::Input::Stream *source)
{
	this->source = source;
}

Companion::Processing::ImageProcessing *Companion::Configuration::getProcessing() const
{

	if (this->processing == nullptr)
	{
		throw Error::Code::no_image_processing_algo_set;
	}

	return this->processing;
}

void Companion::Configuration::setProcessing(Companion::Processing::ImageProcessing *processing)
{
	this->processing = processing;
}

int Companion::Configuration::getSkipFrame() const
{
	return this->skipFrame;
}

void Companion::Configuration::setSkipFrame(int skipFrame)
{

	if (skipFrame <= 0)
	{
		skipFrame = 0;
	}

	this->skipFrame = skipFrame;
}

int Companion::Configuration::getImageBuffer() const
{
	return this->imageBuffer;
}

void Companion::Configuration::setImageBuffer(int imageBuffer)
{

	if (imageBuffer <= 0)
	{
		imageBuffer = 5;
	}

	this->imageBuffer = imageBuffer;
}


void Companion::Configuration::setResultHandler(std::function<SUCCESS_CALLBACK> callback, Companion::ColorFormat colorFormat)
{
	this->callback = callback;
	this->colorFormat = colorFormat;
}

void Companion::Configuration::setErrorHandler(std::function<ERROR_CALLBACK> callback)
{
	this->errorCallback = callback;
}

const std::function<SUCCESS_CALLBACK> &Companion::Configuration::getCallback() const
{

	if (!this->callback)
	{
		throw Error::Code::no_handler_set;
	}

	return this->callback;
}


const std::function<ERROR_CALLBACK> &Companion::Configuration::getErrorCallback() const
{

	if (!this->errorCallback)
	{
		throw Error::Code::no_handler_set;
	}

	return this->errorCallback;
}
