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
const size_t MapDrawer::vert_param_cnt = 2;

const ShaderProgram::Colour RED(1, 0, 0);
const ShaderProgram::Colour GREEN(0, 1, 0);
const ShaderProgram::Colour BLUE(0, 0, 1);

void delay() {
	double u = 12345;
	for(size_t i = 0; i < 100000; ++i)
		for(size_t j = 0; j < 100000; ++j)
			u /= 3;
}

//Tutorial: https://startandroid.ru/ru/uroki/vse-uroki-spiskom/397-urok-168-opengl-vvedenie.html
MapDrawer::MapDrawer() : floor(3), rebind_request(false){
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

void MapDrawer::Render() {
    Log::debug(TAG, "Render");
    if(rebind_request) bindData();
    EGLContext mEglContext = eglGetCurrentContext();
    if(!mEglContext){
        Log::error(TAG, "Zero context");
    }
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAO_);
	storage_->NotifyStartWorking();
	program1_.SetTransformMatrix(storage_->GetTransformMatrix());

	const OpenGLStorage::BufMap& bufmap = storage_->GetBufferMap();
	drawObjects(GL_TRIANGLES, bufmap.P, bufmap, passage_colour_, 3);	//Passages
	drawObjects(GL_LINE_LOOP, bufmap.R, bufmap, room_colour_, 2, 4);	//Rooms
	drawObjects(GL_TRIANGLES, bufmap.PT, bufmap, RED);	//Patches
	drawObjects(GL_LINES, bufmap.L, bufmap, RED, 5);	//LIFTS
	drawObjects(GL_LINES, bufmap.S, bufmap, BLUE, 5);	//STEPS
	drawObjects(GL_LINES, bufmap.PATH, bufmap, RED, 10);	//Path
	drawObjects(GL_TRIANGLES, bufmap.MARK, bufmap, GREEN, 10);

	storage_->NotifyStopWorking();
	glBindVertexArray(0);
}

void MapDrawer::SetFloor(size_t f) {
    floor = f;
    bindData();
}

void MapDrawer::Rebind() {
    bindData();
}

void MapDrawer::drawObjects(const GLenum mode, const size_t type, const OpenGLStorage::BufMap& bufmap,
		const ShaderProgram::Colour& color, const size_t line_width, const size_t step){
	if(!bufmap.IsFilled(type)){
		Log::error(TAG, "EMPTY BUF");
		return;
	}
	program1_.SetVertexColor(color);
	glLineWidth(line_width);
	PointT<size_t> range = (bufmap.GetSectorRange(type))/vert_param_cnt;
	if(step == 0){
		glDrawArrays(mode, range.x, range.y - range.x + 1);
	}else{
		for(int i = range.x; i < range.y; i+=step){
			glDrawArrays(mode, i, step);
		}
	}
}

void MapDrawer::SurfaceChanged(int w, int h) {
    //  Устанавливаем границы обрабатываемой части предоставленной поверхности
    //  Левый верхний (X,Y) и правый нижний (X,Y) углы обьекта
    glViewport(0, 0, w, h);
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
	glClearColor(background_colour_.r_, background_colour_.g_,
	        background_colour_.b_, background_colour_.a_);
	program1_.Generate();               // после перезапуска ошибка в glCompileShader() для второго
	program1_.Use();
    this->bindData();
}

void MapDrawer::bindData() {
    Log::debug(TAG, "bindData()");
    rebind_request = false;
	storage_->NotifyStartWorking();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	float* buf = storage_->GetObjects(floor);
	//float* buf = storage_->GetPassages();
	glBufferData(GL_ARRAY_BUFFER, storage_->GetBufferSize()*sizeof(float),
				buf, GL_STATIC_DRAW);	// загрузили данные в буфер
	buf = nullptr;
	//vbo_size_ = storage_->GetBufferSize();

	glBindVertexArray(VAO_);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
		2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);	// отвязали VAO*/
	storage_->NotifyStopWorking();
}
