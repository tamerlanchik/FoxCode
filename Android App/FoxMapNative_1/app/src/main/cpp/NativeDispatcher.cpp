//
// Created by Andrey on 06.04.2019.
//

#include <jni.h>
#include <cstdlib>
#include "FoxUtilites/Log.h"
#include "Graphics/MapDrawer.h"
#include "Graphics/OpenGLStorage.h"
#include "Database/DBMaster.h"
#include "Database/DataBase.h"
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <string>
#include <Database/Entity.h>
#include "RouteSearch/RouteSearch.h"
#include <array>

const char TAG[] = "NativeDispatcher";

#define DB_FILE "/data/data/com.example.foxmap_native_1/databases/MapDB.db"


MapDrawer map_drawer;
//RouteSearch<float>* route_search;

template<class T>
class RouteSearchMock{
public:
    RouteSearchMock(const std::vector<Hall> &Halls, const std::vector<Room> &Rooms) {}
    int RefreshData(const std::vector<Hall> &Halls, const std::vector<Room> &Rooms) { return 1; }
    int BuildRoute(int StartID, int EndID) {
        route_ = { "Room 302", "Passage 3015", "Passage 3016", "Passage 3017", "Room 307" };
        return route_.size();
    }
    const std::vector<std::string> &GetRoute() { return route_;}
    ~RouteSearchMock(){}

private:
    std::vector<std::string> route_;
};

RouteSearchMock<float>* route_search;

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_init(
        JNIEnv *env, jclass type, jobject asset_manager){
    Log::debug(TAG, "init()");

    AAssetManager *native_asset_manager = AAssetManager_fromJava(env, asset_manager);
    map_drawer.Init(native_asset_manager);
}


JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_surfaceCreated(JNIEnv *env, jclass type){
    OpenGLStorage* s = OpenGLStorage::Get();
    Log::debug(TAG, "Start waiting");
    while(!s->IsInflated()) {}
    Log::debug(TAG, "Stop waiting");
    map_drawer.SurfaceCreated();
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_setFloor(JNIEnv *env, jclass type, jint floor){
    map_drawer.SetFloor(floor);
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_MapDrawerJNI_rebind(JNIEnv *env, jclass type){
    map_drawer.Rebind();
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
        JNIEnv *env, jobject instance, jstring db_path, jobject asset_manager){
    Log::debug(TAG, "Init storage master");
    enum {DBMASTER, ASSET};
    const bool database_src = ASSET;

    if(database_src == DBMASTER){
        class Adapter : public OpenGLStorage::DBAdapter{
            DBMaster* database_;
        public:
            Adapter(DBMaster* db) : database_(db) {};
            Adapter(const char* path) {
                database_ = new DBMaster(path);
            }
            ~Adapter(){
                delete database_;
            }
            std::vector<Hall> GetPassages() override {
                int res = database_->ReadHalls();
                switch(res){
                    case -1:
                        Log::error(TAG, "Cannot open database");
                        break;
                    case -2:
                        Log::error(TAG, "Cannot read from database");
                        break;
                    case 0:
                        return database_->GetHalls();
                }
                return std::vector<Hall>();
            }
            std::vector<Room> GetRooms() override {
                if(database_->ReadRooms() < 0){
                    Log::debug(TAG, "Cannot read rooms");
                    return std::vector<Room>();
                }
                return database_->GetRooms();
            }
        };

        const char* path = env->GetStringUTFChars(db_path, 0);
        Adapter adapter(path);
        env->ReleaseStringUTFChars(db_path, path);

        OpenGLStorage::Get()->NotifyStartWorking();
        bool res = OpenGLStorage::Get()->InflateStorage(adapter);
        OpenGLStorage::Get()->NotifyStopWorking();
        if(res == false){
            return 2;
        }

        route_search = new RouteSearchMock<float>(adapter.GetPassages(), adapter.GetRooms());
    } else{
        class Adapter : public OpenGLStorage::DBAdapter{
        public:
            Adapter(AAssetManager* asset_manager, const std::string& filename){
                db_ = new DataBase(asset_manager, filename);
            }

            Adapter(AAssetManager* asset_manager, const std::vector<std::string>& filenames){
                db_ = new DataBase(asset_manager, filenames);
            }
            std::vector<Hall> GetPassages() override {
                return db_->GetHalls();
            }
            std::vector<Room> GetRooms() override {
                return db_->GetRooms();
            }
            ~Adapter(){
                delete db_;
            }
        private:
            DataBase* db_;
        };

        if(!asset_manager){
            Log::error(TAG, "Zero asset_manager");
            return 1;
        }
        AAssetManager *native_asset_manager = AAssetManager_fromJava(env, asset_manager);
        std::vector<std::string> db_names= {"map_3.txt", "map_4.txt"};
        Adapter adapter(native_asset_manager, db_names);

        OpenGLStorage::Get()->NotifyStartWorking();
        OpenGLStorage::Get()->InflateStorage(adapter);
        OpenGLStorage::Get()->NotifyStopWorking();

        route_search = new RouteSearchMock<float>(adapter.GetPassages(), adapter.GetRooms());
    }


    return 0;
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_StorageMasterJNI_inflateDatabase(
        JNIEnv *env, jobject instance) {
}



JNIEXPORT jboolean JNICALL
Java_com_example_foxmap_1native_11_MapGuide_buildRoute(
        JNIEnv *env, jobject instance, jstring from_name, jstring to_name){

    const char* from = env->GetStringUTFChars(from_name, 0);
    const char* to = env->GetStringUTFChars(to_name, 0);
    try{
        if(!route_search) throw(new std::exception);
        size_t path_size = route_search->BuildRoute(303, 323);
        if(path_size <= 0) throw(new std::exception);

        OpenGLStorage::Get()->NotifyStartWorking();
        OpenGLStorage::Get()->SetRoute(route_search->GetRoute());
        OpenGLStorage::Get()->NotifyStopWorking();
        Log::debug(TAG, "Set route");
        //map_drawer.Rebind();
        map_drawer.rebind_request = true;
        return true;
    }catch (...){
        env->ReleaseStringUTFChars(from_name, from);
        env->ReleaseStringUTFChars(to_name, to);
        return false;
    }
}

JNIEXPORT jboolean JNICALL
Java_com_example_foxmap_1native_11_MapGuide_findOnMap(
        JNIEnv *env, jobject instance, jstring obj_name){

    const char* str = env->GetStringUTFChars(obj_name, 0);
    std::string obj(str);
    bool res = OpenGLStorage::Get()->SetObjectMark(obj);
    Log::debug(TAG, "SetMark");
    env->ReleaseStringUTFChars(obj_name, str);
    return res;
}

}


