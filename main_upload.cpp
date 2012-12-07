#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "CvMongoMat.h"

std::string host       = "localhost:27017";
std::string collection = "test.capture";
std::string key        = "camera0";

int main(int argc, char *argv[])
{
	bool rv;

	// initialize capture device
	cv::VideoCapture capture;
	rv = capture.open(0);
	if (rv == false) {
		std::cerr << "capture.open() failed..." << std::endl;
		return -1;
	}
	capture.set(CV_CAP_PROP_FPS, 30);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	std::cout << "capture start...." << std::endl;

	// connect MongoDB
	mongo::DBClientConnection conn;
	conn.connect(host);

	CvMongoMat mongo_img(conn, collection, key);

	cv::Mat image;
	while(true) {
		// capture image from camera
		capture >> image;

		// upload image to MongoDB
		mongo_img.upload(image);

		if (!image.empty()) {
			cv::imshow("capture", image);
		}
		
		int c = cv::waitKey(33);
		if (c == 27) {
			break;
		}
	}	

	capture.release();
	cv::destroyAllWindows();
	
	return 0;
}

