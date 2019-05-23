cmake_minimum_required(VERSION 3.4.1)

add_library(FoxUtilites STATIC Log.cpp Point.cpp)

target_link_libraries(FoxUtilites log)
