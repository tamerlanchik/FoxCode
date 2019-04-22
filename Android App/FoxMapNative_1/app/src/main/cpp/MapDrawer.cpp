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
    assert(asset_manager);

    //  Получаем вершинный шейдер
    this->triangle_vert_sh_src_ = ShaderMaster::GetShaderRaw(
            asset_manager, triangle_vertex_shader_name_);
    assert(this->triangle_vert_sh_src_.size() > 0);

    //  Получаем фрагментный шейдер
    this->triangle_frag_sh_src_ = ShaderMaster::GetShaderRaw(
            asset_manager, triangle_fragment_shader_name_);
    assert(this->triangle_frag_sh_src_.size() > 0);
}

void MapDrawer::Render() {
    Log::debug(TAG, "Render()");
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(triangle_program_id);

    //  рисуем TRIANLGE, значения берём с 0 индекса для 3 вершин
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    glClearColor(0.698f, 0.843f, 0.784f, 1.f);
    GLuint program_id = ShaderMaster::CreateProgram(
            this->triangle_vert_sh_src_, this->triangle_frag_sh_src_);
    assert(program_id != 0);

    glUseProgram(program_id);

    this->triangle_program_id = program_id;
    this->BindData();

    Log::debug(TAG, "End SurfaceCreated()");
}

void MapDrawer::BindData() {
    int u_color_location = glGetUniformLocation(this->triangle_program_id, "u_Color");
    assert(u_color_location != -1);
    //  Передаём в u_color_location синий цвет (0, 0, 1: 1)
    glUniform4f(u_color_location, 0.0f, 0.0f, 1.0f, 1.0f);
    u_color_location_ = (GLuint) u_color_location;  // signed to unsigned int

    int a_position_location = glGetAttribLocation(this->triangle_program_id, "a_Position");
    assert(a_position_location != -1);
    a_position_location_ = (GLuint) a_position_location;

    //  заполняем шейдер вершин значениями трёх заданных вершин треугольника
    //  у нас есть по 2 координаты на вершину. Шейдер требует 4.
    //  Поэтому передаём кол-во значений=2,
    //  при этом шейдер дозаполнится значениями по умолчанию: {x, y, 0, 1}
    //  0 - количество [передаваемых атрибутов в массиве] минус один
    //  vertex_data_ - указатель на массив координат
    glVertexAttribPointer(a_position_location_, 2, GL_FLOAT, GL_FALSE, 0, vertex_data_);
    glEnableVertexAttribArray(a_position_location_);
}
