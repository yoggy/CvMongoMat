#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "CvMongoMat.h"

std::string host       = "localhost:27017";
std::string collection = "test.capture";
std::string key        = "camera0";

int main(int argc, char *argv[])
{
	bool rv;

	// connect MongoDB
	mongo::DBClientConnection conn;
	conn.connect(host);

	CvMongoMat mongo_img(conn, collection, key);
	cv::Mat image;

	while(true) {
		mongo_img.download(image);
		if (!image.empty()) {
			cv::imshow("download image", image);
		}
		
		int c = cv::waitKey(33);
		if (c == 27) {
			break;
		}
	}	

	cv::destroyAllWindows();
	
	return 0;
}

