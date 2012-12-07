CvMongoMat
===========
OpenCVのcv::Matの画像データをMongoDBへアップロード/ダウンロードするライブラリ。

ネットワーク経由で複数マシンから画像を共有したいときなどにどうぞ。

Requirement
==========
Ubuntu 12.04 Desktop LTSで動作確認しています。
コンパイルには次のパッケージが必要です。

<pre>
    $ sudo apt-get install mongodb mongodb-dev
    $ sudo apt-get install libopencv-dev libboost-all-dev
    $ sudo apt-get install cmake
</pre>

ソースのコンパイルはcmakeを使ってください。

<pre>
    $ git clone https://github.com/yoggy/CvMongoMat.git
    $ cd CvMongoMat
    $ cmake .
    $ make
</pre>

HowToUse
===========
詳細はmain_upload.cpp, main_download.cppを参照。

<pre>
  std::string host       = "localhost:27017";
  std::string collection = "test.capture";
  std::string key        = "camera0";

  // MongoDBへ接続
  mongo::DBClientConnection conn;
  conn.connect(host);

  // connection, collection名、キーを指定してCvMongoMatを用意
  CvMongoMat mongo_img(conn, collection, key);
  
  cv::Mat image;
  cv::VideoCapture capture;
  capture.open(0);
  capture >> image;
  
  // 画像をMongoDBへアップロード
  mongo_img.upload(image);
  
  // 画像をMongoDBからダウンロード
  mongo_img.download(image);
</pre>

