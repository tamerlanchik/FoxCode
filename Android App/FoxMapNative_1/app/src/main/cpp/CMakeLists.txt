# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp)

add_library(
        map_guide
        SHARED
        map_guide.cpp
)

add_library(
        map_drawer
        SHARED
        map_drawer.cpp
)
add_library(
        Log
        SHARED
        Log.cpp
)
add_library(
        MapDrawer
        SHARED
        MapDrawer.cpp
)
add_library(
        ShaderMaster
        SHARED
        ShaderMaster.cpp
)
add_library(
        OpenGLStorage
        SHARED
        OpenGLStorage.cpp
)
add_library(
        MapItemsStorage
        SHARED
        MapItemsStorage.cpp
)
add_library(
        ShaderProgram
        SHARED
        ShaderProgram.cpp
)
add_library(
        DataBase
        SHARED
        DataBase.cpp
)
add_library(
        MapItem
        SHARED
        MapItem.cpp
)


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib
        # Links the target library to the log library
        # included in the NDK.
        android
        log
        ${log-lib})
target_link_libraries(
        map_drawer
        Log
        android
        MapDrawer
        log)
target_link_libraries(
        Log
        android
        log
)
target_link_libraries(
        MapDrawer
        ShaderMaster
        android
        Log
        GLESv2
        ShaderProgram
        OpenGLStorage
        EGL
)

target_link_libraries(
        ShaderMaster
        android
        Log
        GLESv2
)

target_link_libraries(
        ShaderProgram
        android
        Log
        GLESv2
        ShaderMaster
)
target_link_libraries(
        OpenGLStorage
        MapItemsStorage
        DataBase
        GLESv2
)
target_link_libraries(
        MapItemsStorage
        DataBase
        MapItem
)
target_link_libraries(
        DataBase
        android
)