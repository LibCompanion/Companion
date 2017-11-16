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

#include "Util.h"

bool Companion::Util::isImageLoaded(cv::Mat &img)
{
	return !img.empty();
}

void Companion::Util::resizeImage(cv::Mat &img, SCALING scaling)
{
	cv::Point size = getScaling(scaling);
	cv::resize(img, img, cv::Size(size.x, size.y), cv::INTER_AREA);
}

void Companion::Util::ratioPosition(cv::Point &point, int cWidth, int cHeight, int nWidth, int nHeight)
{
	point.x = (int)(((float)point.x / (float)cWidth) * nWidth);
	point.y = (int)(((float)point.y / (float)cHeight) * nHeight);
}

bool Companion::Util::hasDistantPosition(cv::Point2f origin, cv::Point2f point, int distance)
{
	return (std::abs(origin.x - point.x) >= distance) || (std::abs(origin.y - point.y) >= distance);
}

bool Companion::Util::checkDistantDiagonals(cv::Point2f topRight, cv::Point2f bottomLeft, cv::Point2f topLeft, cv::Point2f bottomRight, int threshold, int distance)
{
	double diag0 = cv::norm(topRight-bottomLeft);
	double diag1 = cv::norm(topLeft-bottomRight);
    float diff;

    if (diag0 >= diag1) {
        diff = (1 - (diag1 / diag0)) * 100;
    } else {
        diff = (1 - (diag0 / diag1)) * 100;
    }

	return diff < threshold && (diag0 > distance) && (diag1 > distance);
}

void Companion::Util::convertColor(cv::Mat& src, cv::Mat& dst, Companion::ColorFormat colorFormat)
{
	switch (colorFormat)
	{
	case Companion::ColorFormat::RGB:
		cv::cvtColor(src, dst, cv::ColorConversionCodes::COLOR_BGR2RGB);
		break;
	case Companion::ColorFormat::RGBA:
		cv::cvtColor(src, dst, cv::ColorConversionCodes::COLOR_BGR2RGBA);
		break;
	case Companion::ColorFormat::BGR:
		dst = src;
		break;
	case Companion::ColorFormat::BGRA:
		cv::cvtColor(src, dst, cv::ColorConversionCodes::COLOR_BGR2BGRA);
		break;
	case Companion::ColorFormat::GRAY:
		cv::cvtColor(src, dst, cv::ColorConversionCodes::COLOR_BGR2GRAY);
		break;
	}
}

cv::Point Companion::Util::getScaling(SCALING scaling)
{
	cv::Point size;
	switch (scaling)
	{
	case Companion::SCALING::SCALE_2048x1152:
		size.x = 2048;
		size.y = 1152;
		break;
	case Companion::SCALING::SCALE_1920x1080:
		size.x = 1920;
		size.y = 1080;
		break;
	case Companion::SCALING::SCALE_1600x900:
		size.x = 1600;
		size.y = 900;
		break;
	case Companion::SCALING::SCALE_1408x792:
		size.x = 1408;
		size.y = 792;
		break;
	case Companion::SCALING::SCALE_1344x756:
		size.x = 1344;
		size.y = 756;
		break;
	case Companion::SCALING::SCALE_1280x720:
		size.x = 1280;
		size.y = 720;
		break;
	case Companion::SCALING::SCALE_1152x648:
		size.x = 1152;
		size.y = 648;
		break;
	case Companion::SCALING::SCALE_1024x576:
		size.x = 1024;
		size.y = 576;
		break;
	case Companion::SCALING::SCALE_960x540:
		size.x = 960;
		size.y = 540;
		break;
	case Companion::SCALING::SCALE_896x504:
		size.x = 896;
		size.y = 504;
		break;
	case Companion::SCALING::SCALE_800x450:
		size.x = 800;
		size.y = 450;
		break;
	case Companion::SCALING::SCALE_768x432:
		size.x = 768;
		size.y = 432;
		break;
	case Companion::SCALING::SCALE_640x360:
		size.x = 640;
		size.y = 360;
		break;
	}

	return size;
}
