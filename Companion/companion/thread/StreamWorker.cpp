/*
 * This program is an image recognition library written with OpenCV.
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

#include "StreamWorker.h"

Companion::Thread::StreamWorker::StreamWorker(int buffer, ColorFormat colorFormat)
{
	this->finished = false;
	this->colorFormat = colorFormat;
	this->buffer = buffer;
	if (this->buffer <= 0)
	{
		this->buffer = 1;
	}
}

void Companion::Thread::StreamWorker::Produce(PTR_STREAM stream, int skipFrame, std::function<ERROR_CALLBACK> errorCallback)
{

	int skipFrameNr = 0;
	cv::Mat frame;

	try
	{
		frame = stream->ObtainImage();

		while (!stream->IsFinished())
		{

			if (!frame.empty())
			{
				// If skip frame is not used...
				if (skipFrame <= 0 && StoreFrame(frame))
				{
					// Obtain next frame to store
					frame = stream->ObtainImage();
				}
				else if (skipFrame > 0)
				{
					// ... check if skip frame number is reached and If frame was stored
					if (skipFrameNr == skipFrame && StoreFrame(frame))
					{
						// Obtain next frame to store
						frame = stream->ObtainImage();
						skipFrameNr = 0;
					}
					else if (skipFrameNr != skipFrame)
					{
						frame.release();
						frame = stream->ObtainImage();
						skipFrameNr++;
					}
				}
			}
			else
			{
				// If frames are empty loop
				frame = stream->ObtainImage();
			}
		}

		std::lock_guard<std::mutex> lk(this->mx);
		this->finished = true;
		this->cv.notify_all();

	}
	catch (Error::Code error)
	{
		errorCallback(error);
	}
}

void Companion::Thread::StreamWorker::Consume(PTR_IMAGE_PROCESSING processing, std::function<ERROR_CALLBACK> errorCallback, std::function<SUCCESS_CALLBACK> successCallback)
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
				Util::ConvertColor(frame, resultBGR, this->colorFormat);
				successCallback(processing->Execute(frame), resultBGR);
			}
			catch (Error::Code errorCode)
			{
				// Single error messages from processing
				errorCallback(errorCode);
			}
			catch (Error::CompanionException ex)
			{
				// Multiple error messages only called by parallelized methods
				while (ex.HasNext())
				{
					errorCallback(ex.Next());
				}
			}

			frame.release();
			resultBGR.release();
		}
	}
}

bool Companion::Thread::StreamWorker::StoreFrame(cv::Mat frame)
{
	std::lock_guard<std::mutex> lk(this->mx);
	if (this->queue.size() >= this->buffer)
	{
		// If buffer full try to notify producer and wait current frame and do nothing
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
