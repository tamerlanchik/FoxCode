//
// Created by Andrey on 06.04.2019.
//
#pragma once
#ifndef FOXMAPNATIVE_1_MAPDRAWER_H
#define FOXMAPNATIVE_1_MAPDRAWER_H

#ifdef __ANDROID__
    #include <GLES3/gl3.h>
	#include <android/asset_manager.h>
	#include <EGL/egl.h>
#else
    #define GLEW_STATIC
    #include <GL/glew.h>
#endif
#include "Visitor.h"
#include "OpenGLStorage.h"
#include "ShaderProgram.h"
#include "FoxUtilites/Log.h"
#include <assert.h>


class MapDrawer : public Visitor{
public:
    MapDrawer();
    void Load();
    bool Init();

#ifdef __ANDROID__
    void Init(AAssetManager*);
#endif

    void Render();
    void SurfaceChanged(int w, int h);
    void SurfaceCreated();
private:
	ShaderProgram program1_;
	OpenGLStorage* storage_;
#ifdef __ANDROID__
	AAssetManager* asset_manager_;
#endif

    void bindData();

	void drawPassages();

	void drawRooms();

    void visit(const Room& r) override {

    }
    void visit(const Passage& r) override{

    }
    void visit(const Steps& r) override{

    }


    static const char TAG[];
    static const char triangle_vertex_shader_name_[];
    static const char triangle_fragment_shader_name_[];
};


#endif //FOXMAPNATIVE_1_MAPDRAWER_H
