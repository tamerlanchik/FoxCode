//
// Created by Andrey on 06.04.2019.
//

#include "MapDrawer.h"
#include "ShaderMaster.h"
#include "Log.h"
#include <assert.h>

const char MapDrawer::TAG[] = "MapDrawer";
const char MapDrawer::triangle_vertex_shader_name_[] = "vertex_shader.glsl";
const char MapDrawer::triangle_fragment_shader_name_[] = "fragment_shader.glsl";

//Tutorial: https://startandroid.ru/ru/uroki/vse-uroki-spiskom/397-urok-168-opengl-vvedenie.html
MapDrawer::MapDrawer() {
    map_items_ = MapItemStorage::Get();
}

void MapDrawer::Init() {
    Log::debug(TAG, "Init()");

    //  Получаем вершинный шейдер
    this->triangle_vert_sh_src_ = ShaderMaster::GetShaderRaw(triangle_vertex_shader_name_);
    assert(this->triangle_vert_sh_src_.size() > 0);
	std::cout << this->triangle_vert_sh_src_ << "\n";

    //  Получаем фрагментный шейдер
    this->triangle_frag_sh_src_ = ShaderMaster::GetShaderRaw(triangle_fragment_shader_name_);
    assert(this->triangle_frag_sh_src_.size() > 0);
}

void MapDrawer::Render() {
    Log::debug(TAG, "Render()");
    /*glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(triangle_program_id);

    //  рисуем TRIANLGE, значения берём с 0 индекса для 3 вершин
    glDrawArrays(GL_TRIANGLES, 0, 1);*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data_), vertex_data_, GL_STATIC_DRAW);
	// 1. Затем установим указатели на вершинные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	// 2. Используем нашу шейдерную программу
	glUseProgram(triangle_program_id);
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
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data_), vertex_data_, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
    /*int u_color_location = glGetUniformLocation(this->triangle_program_id, "u_Color");
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
    glEnableVertexAttribArray(a_position_location_);*/
}
