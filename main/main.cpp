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

#include <thread>
#include <companion/openCV3/thread/ProducerStream.h>
#include <companion/openCV3/thread/ConsumerStream.h>
#include <boost/lockfree/spsc_queue.hpp>

int main() {

	//std::string path = "D:/Data/Master/Testcase/HBF/";
    std::string path = "/home/asekulsk/Dokumente/Master/Testcase/HBF/";
    std::string testImg1 = path + std::string("Sample_Middle.jpg");
    std::string testImg2 = path + std::string("Sample_Left.jpg");
    std::string testImg3 = path + std::string("Sample_Right.jpg");
    std::string testVideo = path + std::string("Muelheim_HBF.mp4");
    std::vector<std::string> testImages;
    testImages.push_back(testImg1);
    testImages.push_back(testImg2);
    testImages.push_back(testImg3);

    boost::lockfree::spsc_queue<cv::Mat> queue(30);
    ProducerStream ps(queue);
    ConsumerStream cs(queue);

    std::thread t1(&ProducerStream::run, ps, testVideo);
    std::thread t2(&ConsumerStream::run, cs, testImages);
    t1.join();
    t2.join();

    return 0;
}