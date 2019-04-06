//
// Created by Andrey on 06.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPDRAWER_H
#define FOXMAPNATIVE_1_MAPDRAWER_H

#include <jni.h>
#include <cstdlib>
#include <android/log.h>
#include <android/asset_manager_jni.h>


class MapDrawer{
public:
    MapDrawer() {};
    void Init( AAssetManager* );
    void Render();
    void SurfaceChanged(int w, int h);
    void SurfaceCreated();
private:
    static const char TAG[];
};
const char MapDrawer::TAG[] = "MapDrawer";


#endif //FOXMAPNATIVE_1_MAPDRAWER_H
