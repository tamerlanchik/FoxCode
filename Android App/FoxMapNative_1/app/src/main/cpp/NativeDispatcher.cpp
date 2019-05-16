//
// Created by Andrey on 06.04.2019.
//

#include <jni.h>
#include <cstdlib>
#include "FoxUtilites/Log.h"
#include "Graphics/MapDrawer.h"
#include "Graphics/OpenGLStorage.h"
#include "Database/DBMaster.h"
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <string>
#include <Database/sqlite_lib/sqlite3ndk.h>
#include <Database/sqlite_lib/sqlite3.h>

const char TAG[] = "NativeDispatcher";

#define DB_FILE "/data/data/com.example.foxmap_native_1/databases/MapDB.db"


MapDrawer map_drawer;
DBMaster* database = nullptr;

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

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_StorageMasterJNI_init(
        JNIEnv *env, jobject instance, jobject asset_manager){
    assert(asset_manager);
    const char filename[] = "MapDB.db";
    Log::debug(TAG, "Init storage master");
    if(!database)
        database = new DBMaster(DB_FILE);
    else
        Log::debug(TAG, "Database is already created");
    if(!database){
        Log::error(TAG, "Cannot create database!");
        return;
    } else
        Log::debug(TAG, "Created database");
    AAssetManager *native_asset_manager = AAssetManager_fromJava(env, asset_manager);
    /*if(sqlite3_ndk_init(native_asset_manager) != SQLITE_OK)
    {
        Log::error(TAG, "Cannot init sqlite3ndk");
        return;
    }else{
        Log::debug(TAG, "sqlite3ndk init OK");
    }*/
    AAsset* asset = AAssetManager_open(native_asset_manager, filename, AASSET_MODE_STREAMING);
    if(!asset){
        Log::error(TAG, "Cannot open asset-database");
        return;
    }
    sqlite3 *db;
    int errCode = sqlite3_open_v2(DB_FILE, &db, SQLITE_OPEN_READWRITE, NULL);
    if (SQLITE_OK != errCode) {
        sqlite3_close(db);
        Log::error(TAG, (std::string("Cannot open database: ") + std::to_string(errCode)).c_str());
        return;
    }else{
        Log::debug(TAG, "Database opened");
    }

    if(database->ReadHalls() < 0) {
        Log::error(TAG, "Cannot read halls from database");
    }
    else Log::debug(TAG, (std::string("Data halls read: ") + std::to_string(database->GetHalls().size())).c_str());

    if(database->ReadRooms() < 0) {
        Log::error(TAG, "Cannot read rooms from database");
    }
    else Log::debug(TAG, (std::string("Data rooms read: ") + std::to_string(database->GetRooms().size())).c_str());
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_StorageMasterJNI_inflateDatabase(
        JNIEnv *env, jobject instance) {
}
}

