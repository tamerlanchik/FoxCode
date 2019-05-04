//
// Created by Andrey on 06.04.2019.
//

#include "MapDrawer.h"
#include "ShaderMaster.h"
#include "Log.h"
#include <assert.h>
#include "DataBase.h"

const char MapDrawer::TAG[] = "MapDrawer";
const char MapDrawer::triangle_vertex_shader_name_[] = "vertex_shader.glsl";
const char MapDrawer::triangle_fragment_shader_name_[] = "fragment_shader.glsl";

//Tutorial: https://startandroid.ru/ru/uroki/vse-uroki-spiskom/397-urok-168-opengl-vvedenie.html
MapDrawer::MapDrawer() {
    map_items_ = MapItemStorage::Get();
}

void MapDrawer::Init() {
    Log::debug(TAG, "Init()");

	DataBase* database = new DataBase();
	map_items_->SetDatabase(database);
	map_items_->InflateStorage();

    //  Получаем вершинный шейдер
    this->triangle_vert_sh_src_ = ShaderMaster::GetShaderRaw(triangle_vertex_shader_name_);
    assert(this->triangle_vert_sh_src_.size() > 0);
	std::cout << this->triangle_vert_sh_src_ << "\n";

    //  Получаем фрагментный шейдер
    this->triangle_frag_sh_src_ = ShaderMaster::GetShaderRaw(triangle_fragment_shader_name_);
    assert(this->triangle_frag_sh_src_.size() > 0);

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
}

void MapDrawer::Render() {
    //Log::debug(TAG, "Render()");
    /*glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(triangle_program_id);

    //  рисуем TRIANLGE, значения берём с 0 индекса для 3 вершин
    glDrawArrays(GL_TRIANGLES, 0, 1);*/
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data_), vertex_data_, GL_STATIC_DRAW);
	// 1. Затем установим указатели на вершинные атрибуты
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	// 2. Используем нашу шейдерную программу
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	GLfloat timeValue = glfwGetTime();
	GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
	//GLint vertexColorLocation = glGetUniformLocation(triangle_program_id, "u_color");
	//std::cout << "\n" << vertexColorLocation << "\n";
	glUseProgram(triangle_program_id);
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glUniform3f(0, 0.0f, greenValue, 0.0f);
	glDrawArrays(GL_LINES, 0, 8);
	//glDrawArrays(GL_LINE_STRIP, 6, 6);
	//glDrawArrays(GL_LINE_LOOP, 4, 4);
	glBindVertexArray(0);
    //Log::debug(TAG, "Render() - 1");
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
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	float* buf = map_items_->GetRooms();
	//float vertex_data_[6] = { -1, -1, -1, 1, 0, 0 };
	glBufferData(GL_ARRAY_BUFFER, map_items_->GetBufferSize()*sizeof(float),
				buf, GL_STATIC_DRAW);	// загрузили данные в буфер
	buf = nullptr;


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE,
				2*sizeof(GLfloat), (GLvoid*)0);	// связываем вершинные атрибуты
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	glBindVertexArray(0);	// отвязали VAO

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
