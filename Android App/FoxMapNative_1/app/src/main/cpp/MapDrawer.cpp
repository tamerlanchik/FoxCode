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

bool MapDrawer::Init() {
    Log::debug(TAG, "Init()");
	Log::info("OpenGL Version", (const char*)glGetString(GL_VERSION));
	Log::info("GLSL Version", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	try {
		DataBase* database;
		database = new DataBase();
		if (!storage_)
			throw std::runtime_error("Null pointer to storage");
		storage_->SetDatabase(database);
		storage_->InflateStorage();
		program1_ = ShaderProgram(triangle_vertex_shader_name_, triangle_fragment_shader_name_);
	}
	catch (const std::exception& e) {
		Log::error(TAG, e.what());
		return 0;
	}
	catch (...) {
		Log::error(TAG, "Unknown error");
		return 0;
	}
	return true;
}
#ifdef __ANDROID__
void MapDrawer::Init(AAssetManager* asset_manager){
	Log::debug(TAG, "Init()");
	Log::info("OpenGL Version", (const char*)glGetString(GL_VERSION));
	Log::info("GLSL Version", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    DataBase* database = new DataBase(asset_manager);
	storage_->SetDatabase(database);
	storage_->InflateStorage();
	program1_ = ShaderProgram(asset_manager, triangle_vertex_shader_name_, triangle_fragment_shader_name_);
}
#endif
void MapDrawer::Render() {
	#ifdef __ANDROID__
	Log::debug(TAG, "Render");
	#endif
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(storage_->GetVaoPassage());
	program1_.SetVertexColor(0, 0, 1);
	program1_.SetTransformMatrix(storage_->GetTransformMatrix());

	drawPassages();
	drawRooms();

	glBindVertexArray(0);
}

void MapDrawer::drawPassages() {
	glLineWidth(3);
	glDrawArrays(GL_LINES, 0, storage_->GetPassagesBufSize() / 2);
}

void MapDrawer::drawRooms() {
	glLineWidth(1);
	program1_.SetVertexColor(0, 0, 0);
	for (int i = storage_->GetPassagesBufSize() / 2;
		i < storage_->GetBufferSize(); i += 4) {

		glDrawArrays(GL_LINE_LOOP, i, 4);
	}
}

void MapDrawer::SurfaceChanged(int w, int h) {
    //  Устанавливаем границы обрабатываемой части предоставленной поверхности
    //  Левый верхний (X,Y) и правый нижний (X,Y) углы обьекта
    glViewport(0, 0, w, h);
    assert(storage_);
    storage_->UpdateScreenDimensions(w,h);
    Log::debug(TAG, "SurfaceChanged()");
}

void MapDrawer::SurfaceCreated() {
    Log::debug(TAG, "SurfaceCreated()");
    //glClearColor(0.698f, 0.843f, 0.784f, 1.f);
	glClearColor(1, 1, 1, 1.f);
	program1_.Generate();
	program1_.Use();
    this->BindData();
}

void MapDrawer::BindData() {
	glBindBuffer(GL_ARRAY_BUFFER, storage_->GetVbo());
	float* buf = storage_->GetObjects();
	//float* buf = storage_->GetPassages();
	glBufferData(GL_ARRAY_BUFFER, storage_->GetBufferSize()*sizeof(float),
				buf, GL_STATIC_DRAW);	// загрузили данные в буфер
	buf = nullptr;
	storage_->SetVboSize(storage_->GetBufferSize());

	glBindVertexArray(storage_->GetVaoPassage());
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
				2*sizeof(GLfloat), (GLvoid*)0);	// связываем вершинные атрибуты
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);	// отвязали VAO

	/*glBindVertexArray(storage_->GetVaoRoom());
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
		2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);	// отвязали VAO*/
}
