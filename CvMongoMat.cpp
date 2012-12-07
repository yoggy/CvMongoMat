#include "CvMongoMat.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>

long long get_timestamp_()
{   
    boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration d(t.time_of_day());
    return d.total_milliseconds();
}

CvMongoMat::CvMongoMat(mongo::DBClientConnection &conn, const std::string &collection, const std::string &key)
	: conn_(&conn), collection_(collection), key_(key), timestamp_(0)
{
	if (conn_ == NULL) return;

	// cleanup target key
	mongo::BSONObj remove_query = BSON(key_ << BSON("$exists" << "true"));
	conn_->remove(collection_, remove_query);

	// insert dummy key-value
	mongo::BSONObjBuilder builder;
	builder.appendBinData(key_, 0, mongo::BinDataGeneral, NULL);
	builder.append("timestamp", get_timestamp_());
	conn_->insert(collection_, builder.obj());
}

CvMongoMat::~CvMongoMat()
{

}

bool CvMongoMat::upload(const cv::Mat &image)
{
	if (conn_ == NULL) return false;

	mongo::BSONObj old_obj = BSON(key_ << BSON("$exists" << true));

	mongo::BSONObjBuilder builder;
	if (image.empty()) {
		builder.appendBinData(key_, 0, mongo::BinDataGeneral, NULL);
	}
	else {
		std::vector<int> params(2);
		params[0] = CV_IMWRITE_JPEG_QUALITY;
		params[1] = 95;
		std::vector<uchar> buf;
		cv::imencode(".jpg", image, buf, params);
		builder.appendBinData(key_, buf.size(), mongo::BinDataGeneral, &buf[0]);
	}

	timestamp_ = get_timestamp_();
	builder.append("timestamp", timestamp_);
	conn_->update(collection_, old_obj, builder.obj());

	return true;
}

bool CvMongoMat::download(cv::Mat &image)
{
	image.release();

	if (conn_ == NULL) return false;

	std::auto_ptr<mongo::DBClientCursor> cursor;
	cursor = conn_->query(collection_, QUERY(key_ << BSON("$exists" << true)));

	if (cursor->more() == false) {
		// notiong to do...
		return true;
	}

	mongo::BSONObj obj = cursor->next();
	timestamp_ = obj["timestamp"].Long();

	int len;
	uchar *data = (uchar*)obj[key_].binData(len);
	if (len == 0) return true;

	std::vector<uchar> v(data, data+len);
	image = cv::imdecode(cv::Mat(v), -1);

	return true;
}

long long CvMongoMat::timestamp()
{
	return this->timestamp_;
}

