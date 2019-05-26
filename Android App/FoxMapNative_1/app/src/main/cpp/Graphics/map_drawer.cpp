//
// Created by Andrey on 06.04.2019.
//

#include <jni.h>
#include <cstdlib>
#include "FoxUtilites/Log.h"
#include "Graphics/MapDrawer.h"
#include "Graphics/OpenGLStorage.h"
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <string>

const char TAG[] = "map_drawer";


MapDrawer map_drawer;

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_init(
        JNIEnv *env, jclass type, jobject asset_manager){
    Log::debug(TAG, "init()");

    AAssetManager *native_asset_manager = AAssetManager_fromJava(env, asset_manager);
    map_drawer.Init(native_asset_manager);
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_load(JNIEnv *env, jclass type){
    Log::debug(TAG, "load()");
    map_drawer.Load();
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_surfaceCreated(JNIEnv *env, jclass type){
    map_drawer.SurfaceCreated();
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_surfaceChanged(
        JNIEnv *env, jclass type, jint w, jint h){
    map_drawer.SurfaceChanged(w,h);
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_drawFrame(JNIEnv *env, jclass type){
    map_drawer.Render();
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_onResume(JNIEnv *env, jclass type){
    Log::debug(TAG, "onResume()");
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_onPause(JNIEnv *env, jclass type){
    Log::debug(TAG, "onPause()");
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_commitMapMovement(JNIEnv *env, jclass type,
        jfloat dx, jfloat dy){
    OpenGLStorage::Get()->CommitMapMovement(dx, dy);
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_commitMapZoom(JNIEnv *env, jclass type,
                                                                  jfloat dz){
    OpenGLStorage::Get()->CommitMapZoom(dz);
}

}