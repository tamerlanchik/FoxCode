cmake_minimum_required(VERSION 3.4.1)

include_directories(${CMAKE_SOURCE_DIR})
if(NOT TARGET sqlite_lib)
    add_subdirectory(sqlite_lib)
endif()
if(NOT TARGET Database)
    add_library(Database STATIC
            DataBase.cpp
            DBMaster.cpp
    )
target_link_libraries(Database sqlite_lib FoxUtilites android)
endif()

