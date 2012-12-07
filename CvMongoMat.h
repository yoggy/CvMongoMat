#ifndef __CV__MONGO__MAT__
#define __CV__MONGO__MAT__

#include <opencv2/highgui/highgui.hpp>
#include <mongo/client/dbclient.h>

class CvMongoMat
{
public:
	CvMongoMat(mongo::DBClientConnection &conn, const std::string &collection, const std::string &key);
	virtual ~CvMongoMat();

	bool upload(const cv::Mat &image);
	bool download(cv::Mat &image);

	long long timestamp();

private:
	mongo::DBClientConnection *conn_;
	std::string collection_;
	std::string key_;
	long long timestamp_;
};

#endif // #define __CV__MONGO__MAT__
