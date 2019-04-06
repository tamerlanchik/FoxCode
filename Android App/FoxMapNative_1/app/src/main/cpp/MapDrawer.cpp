//
// Created by Andrey on 06.04.2019.
//

#include "MapDrawer.h"
#include "Log.h"
#include <dlfcn.h>
#include <jni.h>
#include <GLES2/gl2.h>

void MapDrawer::Init(AAssetManager* asset_manager) {
    Log::debug(TAG, "Init()");
}

void MapDrawer::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void MapDrawer::SurfaceChanged(int w, int h) {
    glViewport(0, 0, w, h);
    Log::debug(TAG, "SurfaceChanged()");
}

void MapDrawer::SurfaceCreated() {
    glClearColor(0.f, 1.f, 0.f, 1.f);
    Log::debug(TAG, "SurfaceCreated()");
}
