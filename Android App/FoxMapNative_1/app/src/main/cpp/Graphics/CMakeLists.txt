cmake_minimum_required(VERSION 3.4.1)
include_directories(${CMAKE_SOURCE_DIR})
if (NOT TARGET FoxUtilites)
add_subdirectory(${CMAKE_SOURCE_DIR}/FoxUtilites Graphics/Bin)
    endif()
if (NOT TARGET Database)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Database Graphics/Bin)
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
        android
        GLESv3
        EGL)