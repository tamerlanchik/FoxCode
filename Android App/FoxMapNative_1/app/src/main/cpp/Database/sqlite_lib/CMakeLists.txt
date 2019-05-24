cmake_minimum_required(VERSION 3.4.1)

add_library(sqlite_lib
        SHARED
        sqlite3.c)
target_link_libraries(sqlite_lib android)