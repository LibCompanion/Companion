#include <thread>
#include <companion/openCV3/thread/ProducerStream.h>
#include <companion/openCV3/thread/ConsumerStream.h>

int main() {

	std::string path = "/home/asekulsk/Dokumente/Master/Testcase/HBF/";
    std::string testImg = path + std::string("Sample_Middle.jpg");
    std::string testVideo = path + std::string("Muelheim_HBF.mp4");

    wqueue<cv::Mat> queue;
    ProducerStream ps(queue);
    ConsumerStream cs(queue);

    std::thread t1(&ProducerStream::run, ps, testVideo);
    std::thread t2(&ConsumerStream::run, cs, testImg);
    t1.join();
    t2.join();

	return 0;
}
