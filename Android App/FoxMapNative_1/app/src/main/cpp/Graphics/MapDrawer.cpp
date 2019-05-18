//
// Created by Andrey on 06.04.2019.
//

#include "MapDrawer.h"
#include <unistd.h>

const char MapDrawer::TAG[] = "MapDrawer";
const char MapDrawer::triangle_vertex_shader_name_[] = "vertex_shader.glsl";
const char MapDrawer::triangle_fragment_shader_name_[] = "fragment_shader.glsl";
const ShaderProgram::Colour MapDrawer::passage_colour_(0.9, 0.9, 0.9);
const ShaderProgram::Colour MapDrawer::room_colour_(0, 0, 1);
const ShaderProgram::Colour MapDrawer::background_colour_(1, 1, 1);

void delay() {
	double u = 12345;
	for(size_t i = 0; i < 100000; ++i)
		for(size_t j = 0; j < 100000; ++j)
			u /= 3;
}

//Tutorial: https://startandroid.ru/ru/uroki/vse-uroki-spiskom/397-urok-168-opengl-vvedenie.html
MapDrawer::MapDrawer() {
	storage_ = OpenGLStorage::Get();
}

bool MapDrawer::Init() {
    Log::debug(TAG, "Init()");
	Log::info("OpenGL Version", (const char*)glGetString(GL_VERSION));
	Log::info("GLSL Version", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	Log::info(TAG, "Go sleep 10 s");
	sleep(10);
	Log::info(TAG, "Continue");

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

    EGLContext mEglContext = eglGetCurrentContext();
    if(!mEglContext){
        Log::error(TAG, "Zero context");
    }


    /*DataBase* database = new DataBase(asset_manager);
	storage_->SetDatabase(database);
	storage_->InflateStorage();*/
	program1_ = ShaderProgram(asset_manager, triangle_vertex_shader_name_, triangle_fragment_shader_name_);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO_);
    glGenVertexArrays(1, &VAO_room_);
}
#endif

void MapDrawer::Load() {
    EGLContext mEglContext = eglGetCurrentContext();
    if(!mEglContext){
        Log::error(TAG, "Zero context");
    }
    //assert(mEglContext);

    Log::info(TAG, "Go sleep 10 s");
    //sleep(10);
    storage_->NotifyStartWorking();
    double t = 343;
    for(size_t i = 0; i < 10000; ++i){
        for(size_t j = 0; j < 10000; ++j){
            t /= 45;
        }
        //if(i%100 == 0)
        //Log::debug(TAG, std::to_string(storage_->GetData()).c_str());
    }
    storage_->NotifyStopWorking();
    Log::info(TAG, "Continue");

}
void MapDrawer::Render() {
	#ifdef __ANDROID__
	Log::debug(TAG, "Render");
	#endif
	glClear(GL_COLOR_BUFFER_BIT);

	storage_->NotifyStartWorking();
	program1_.SetTransformMatrix(storage_->GetTransformMatrix());

	glBindVertexArray(VAO_);
	drawPassages();
	drawRooms();
	drawPatches();
	storage_->NotifyStopWorking();
    EGLContext mEglContext = eglGetCurrentContext();
    if(!mEglContext){
        Log::error(TAG, "Zero context");
    }
	glBindVertexArray(0);
}

void MapDrawer::drawPassages() {
	glLineWidth(3);
    program1_.SetVertexColor(0.9, 0.9, 0.9);
	/*GLuint ind[] = {
	        0, 1, 2,
	        0, 2, 3
	};
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawRangeElements(GL_TRIANGLES, 0, 12, 6, GL_UNSIGNED_INT, 0);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 2);*/
	//glDrawArrays(GL_LINES, 0, storage_->GetPassagesBufSize() / 2);
    glDrawArrays(GL_TRIANGLES, 0, storage_->GetBufferMap().GetPassagesCount() / 2);

}

void MapDrawer::drawRooms() {
	glLineWidth(2);
	program1_.SetVertexColor(0, 0, 1);
	/*for (int i = storage_->GetPassagesBufSize()/2;
		i < storage_->GetBufferSize(); i += 4) {

		glDrawArrays(GL_LINE_LOOP, i, 4);
	}*/
	PointT<size_t> range = storage_->GetBufferMap().GetRoomsRange();
	const size_t step = 4;
	for(int i = range.x/2; 2*i < range.y; i+=step){
		glDrawArrays(GL_LINE_LOOP, i, step);
	}
}

void MapDrawer::drawPatches() {
    program1_.SetVertexColor(1, 0, 0);
	PointT<size_t> range = storage_->GetBufferMap().GetPatchesRange();
	glDrawArrays(GL_TRIANGLES, range.x/2, range.y-range.x);
}

void MapDrawer::SurfaceChanged(int w, int h) {
    //  Устанавливаем границы обрабатываемой части предоставленной поверхности
    //  Левый верхний (X,Y) и правый нижний (X,Y) углы обьекта
    glViewport(0, 0, w, h);
    //assert(storage_);
    storage_->NotifyStartWorking();
    storage_->UpdateScreenDimensions(w,h);
    storage_->NotifyStopWorking();
    Log::debug(TAG, "SurfaceChanged()");
}

void MapDrawer::SurfaceCreated() {
    EGLContext mEglContext = eglGetCurrentContext();
    if(!mEglContext){
        Log::error(TAG, "Zero context");
    }
    Log::debug(TAG, "SurfaceCreated()");
    //glClearColor(0.698f, 0.843f, 0.784f, 1.f);
	glClearColor(1, 1, 1, 1.f);
	program1_.Generate();               // после перезапуска ошибка в glCompileShader() для второго
	program1_.Use();
    this->bindData();
}

void MapDrawer::bindData() {
	storage_->NotifyStartWorking();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	float* buf = storage_->GetObjects();
	//float* buf = storage_->GetPassages();
	glBufferData(GL_ARRAY_BUFFER, storage_->GetBufferSize()*sizeof(float),
				buf, GL_STATIC_DRAW);	// загрузили данные в буфер
	buf = nullptr;
	//storage_->SetVboSize(storage_->GetBufferSize());
	vbo_size_ = storage_->GetBufferSize();

	glBindVertexArray(VAO_);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
		2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);	// отвязали VAO*/
	storage_->NotifyStopWorking();
}
