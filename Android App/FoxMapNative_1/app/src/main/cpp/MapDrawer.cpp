//
// Created by Andrey on 06.04.2019.
//

#include "MapDrawer.h"
#include "ShaderMaster.h"
#include "Log.h"
#include <dlfcn.h>
#include <jni.h>
#include <GLES2/gl2.h>
#include <assert.h>

//Tutorial: https://startandroid.ru/ru/uroki/vse-uroki-spiskom/397-urok-168-opengl-vvedenie.html

void MapDrawer::Init(AAssetManager* asset_manager) {
    Log::debug(TAG, "Init()");

    //  Получаем вершинный шейдер
    this->triangle_vert_sh_src = ShaderMaster::GetShaderRaw(
            asset_manager, triangle_vertex_shader_name_);

    //  Получаем фрагментный шейдер
    this->triangle_frag_sh_src = ShaderMaster::GetShaderRaw(
            asset_manager, triangle_fragment_shader_name_);
}

void MapDrawer::Render() {
    Log::debug(TAG, "Render()");
    glClear(GL_COLOR_BUFFER_BIT);
    Log::debug(TAG, "Render() - 1");
}

void MapDrawer::SurfaceChanged(int w, int h) {
    //  Устанавливаем границы обрабатываемой части предоставленной поверхности
    //  Левый верхний (X,Y) и правый нижний (X,Y) углы обьекта
    glViewport(0, 0, w, h);
    Log::debug(TAG, "SurfaceChanged()");
}

void MapDrawer::SurfaceCreated() {
    Log::debug(TAG, "Start SurfaceCreated()");
    glClearColor(0.f, 1.f, 0.f, 1.f);
    GLuint program_id = ShaderMaster::CreateProgram(this->triangle_vert_sh_src, this->triangle_frag_sh_src);
    assert(program_id != 0);

    glUseProgram(program_id);

    Log::debug(TAG, "End SurfaceCreated()");
}
