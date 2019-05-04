//
// Created by Andrey on 06.04.2019.
//

#include "MapDrawer.h"



const char MapDrawer::TAG[] = "MapDrawer";
const char MapDrawer::triangle_vertex_shader_name_[] = "vertex_shader.glsl";
const char MapDrawer::triangle_fragment_shader_name_[] = "fragment_shader.glsl";

//Tutorial: https://startandroid.ru/ru/uroki/vse-uroki-spiskom/397-urok-168-opengl-vvedenie.html
MapDrawer::MapDrawer() {
	storage_ = OpenGLStorage::Get();
}

void MapDrawer::Init() {
    Log::debug(TAG, "Init()");

	DataBase* database = new DataBase();
	storage_->SetDatabase(database);
	storage_->InflateStorage();
	program1_ = ShaderProgram(triangle_vertex_shader_name_, triangle_fragment_shader_name_);
}
#ifdef __ANDROID__
void MapDrawer::Init(AAssetManager* asset_manager){
    EGLContext mEglContext = eglGetCurrentContext();
    std::cout << "Init()\n";
    DataBase* database = new DataBase(asset_manager);
	storage_->SetDatabase(database);
	storage_->InflateStorage();
	program1_ = ShaderProgram(asset_manager, triangle_vertex_shader_name_, triangle_fragment_shader_name_);
}
#endif
void MapDrawer::Render() {
	Log::debug(TAG, "Render-start");
	glClear(GL_COLOR_BUFFER_BIT);
	//program1_.Use();
	//glBindVertexArray(storage_->GetVao());
	program1_.SetVertexColor(0, 1, 0);
	program1_.SetTransformMatrix(storage_->GetTransformMatrix());
	//glDrawArrays(GL_LINES, 0, storage_->GetVboSize());
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
						  2*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINES, 0, 100);
	//glBindVertexArray(0);
	Log::debug(TAG, "Render-finish");
}

void MapDrawer::SurfaceChanged(int w, int h) {
    //  Устанавливаем границы обрабатываемой части предоставленной поверхности
    //  Левый верхний (X,Y) и правый нижний (X,Y) углы обьекта
    glViewport(0, 0, w, h);
    Log::debug(TAG, "SurfaceChanged()");
}

void MapDrawer::SurfaceCreated() {
    Log::debug(TAG, "Start SurfaceCreated()");
    //glClearColor(0.698f, 0.843f, 0.784f, 1.f);
	glClearColor(1, 1, 1, 1.f);
	program1_.Generate();
	program1_.Use();
    this->BindData();
	glLineWidth(3);

    Log::debug(TAG, "End SurfaceCreated()");
}

void MapDrawer::BindData() {
	//glBindVertexArray(storage_->GetVao());
	glBindBuffer(GL_ARRAY_BUFFER, storage_->GetVbo());
	float* buf = storage_->GetRooms();
	glBufferData(GL_ARRAY_BUFFER, storage_->GetBufferSize()*sizeof(float),
				buf, GL_STATIC_DRAW);	// загрузили данные в буфер
	buf = nullptr;
	storage_->SetVboSize(storage_->GetBufferSize());

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
				2*sizeof(GLfloat), (GLvoid*)0);	// связываем вершинные атрибуты
	glEnableVertexAttribArray(0);
	//glBindVertexArray(0);	// отвязали VAO
}
