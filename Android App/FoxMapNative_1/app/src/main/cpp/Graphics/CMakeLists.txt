cmake_minimum_required(VERSION 3.4.1)
include_directories(${CMAKE_SOURCE_DIR})
if (NOT TARGET FoxUtilites)
add_subdirectory(${CMAKE_SOURCE_DIR}/FoxUtilites Graphics/Bin)
    endif()
if (NOT TARGET Database)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Database Graphics/Bin)
endif()
if (NOT TARGET RouteSearch)
    add_subdirectory(${CMAKE_SOURCE_DIR}/RouteSearch Graphics/Bin)
endif()
add_library(Graphics SHARED
    MapDrawer.cpp
    MapItem.cpp
    MapItemsStorage.cpp
    OpenGLStorage.cpp
    ShaderProgram.cpp
    ShaderMaster.cpp
)

target_link_libraries(Graphics
        FoxUtilites
        Database
        RouteSearch
        android
        GLESv3
        EGL)