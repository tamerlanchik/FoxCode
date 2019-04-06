//
// Created by Andrey on 06.04.2019.
//

#include <jni.h>
#include <cstdlib>
#include "Log.h"
#include <android/log.h>

const char TAG[] = "MapDrawer";

class MapDrawer{
public:
    MapDrawer() {};
    void init();
    void render();
};

MapDrawer map_drawer;

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_init(
        JNIEnv *env, jclass type){
    Log::debug(TAG, "init()");
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_surfaceCreated(JNIEnv *env, jclass type){

}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_surfaceChanged(JNIEnv *env, jclass type){

}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_drawFrame(JNIEnv *env, jclass type){

}
}
