cmake_minimum_required(VERSION 2.8)

project( cvmongomat )

set(src_upload main_upload.cpp CvMongoMat.cpp)
set(src_download main_download.cpp CvMongoMat.cpp)

set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_C_FLAGS "-g")

find_package( Boost REQUIRED COMPONENTS system-mt filesystem-mt thread-mt program_options-mt)
find_package( OpenCV REQUIRED )

add_executable( cvmongomat_upload ${src_upload} )
include_directories( /usr/local/include ${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIR} )
target_link_libraries( cvmongomat_upload pthread mongoclient ${Boost_LIBRARIES} ${OpenCV_LIBS} )

add_executable( cvmongomat_download ${src_download} )
include_directories( /usr/local/include ${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIR} )
target_link_libraries( cvmongomat_download pthread mongoclient ${Boost_LIBRARIES} ${OpenCV_LIBS} )

