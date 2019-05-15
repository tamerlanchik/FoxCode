cmake_minimum_required(VERSION 3.14)
project(PROJECT)

set(CMAKE_CXX_STANDARD 14)

add_executable(PROJECT main.cpp DBMaster.h Server.h Server.cpp)