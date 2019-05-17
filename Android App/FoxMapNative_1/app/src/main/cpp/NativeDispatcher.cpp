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
#include <Database/Entity.h>

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

JNIEXPORT jint JNICALL
Java_com_example_foxmap_1native_11_StorageMasterJNI_init(
        JNIEnv *env, jobject instance, jstring db_path){
    Log::debug(TAG, "Init storage master");

    if(!database) {
        const char* path = env->GetStringUTFChars(db_path, 0);
        database = new DBMaster(path);
        env->ReleaseStringUTFChars(db_path, path);
        if(!database){
            return 1;
        } else
            Log::debug(TAG, "Created database");
    }
    else
        Log::debug(TAG, "Database is already created");

    class Adapter : public OpenGLStorage::DBAdapter{
        DBMaster& database_;
    public:
        Adapter(DBMaster& db) : database_(db) {};
        std::vector<Hall> GetPassages() {
            if(database_.ReadHalls() <= 0)
                Log::debug(TAG, "Cannot read halls");
            return database_.GetHalls();
        }
        std::vector<Room> GetRooms() {
            if(database_.ReadRooms() <= 0)
                Log::debug(TAG, "Cannot read rooms");
            return database_.GetRooms();
        }
    };

    Adapter adapter(*database);
    OpenGLStorage::Get()->NotifyStartWorking();
    OpenGLStorage::Get()->InflateStorage(adapter);
    OpenGLStorage::Get()->NotifyStopWorking();

    /*if(database->ReadHalls() < 0) {
        Log::error(TAG, "Cannot read halls from database");
    }
    else Log::debug(TAG, (std::string("Data halls read: ") + std::to_string(database->GetHalls().size())).c_str());

    if(database->ReadRooms() < 0) {
        Log::error(TAG, "Cannot read rooms from database");
        return 1;
    }
    else Log::debug(TAG, (std::string("Data rooms read: ") + std::to_string(database->GetRooms().size())).c_str());
    //Log::debug(TAG, std::to_string(OpenGLStorage::Get()->GetVboSize()).c_str());
    OpenGLStorage::Get()->NotifyStartWorking();
    OpenGLStorage::Get()->NotifyStopWorking();*/
    return 0;
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_StorageMasterJNI_inflateDatabase(
        JNIEnv *env, jobject instance) {
}
}

