cmake_minimum_required(VERSION 2.8)
project(lines )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( lines lines.cpp )
target_link_libraries( lines ${OpenCV_LIBS} )

#add_executable( raspi  raspi_1.cpp )
#target_link_libraries( raspi ${OpenCV_LIBS} )



#add_executable( example test_oflow_sparse.cpp )
#target_link_libraries( example ${OpenCV_LIBS} )
