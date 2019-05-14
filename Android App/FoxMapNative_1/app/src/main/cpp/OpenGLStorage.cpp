#include "OpenGLStorage.h"
#include <unistd.h>

const char OpenGLStorage::TAG[] = "OpenGLStorage";

OpenGLStorage::OpenGLStorage() : screen_dimensions_(Point(0,0))
{
	normalizing_matrix_ = glm::mat4(1.0);
	moving_matrix_ = glm::mat4(1.0f);
	scaling_matrix_ = glm::mat4(1.0f);
	updateTransformMatrix();
	Log::debug(TAG, "OpenGLStorage created!\n");
}
OpenGLStorage* OpenGLStorage::Get() {
	static OpenGLStorage instance_;
	return &instance_;
 }

bool OpenGLStorage::InflateStorage() {
	if (!MapItemStorage::InflateStorage())
		return false;
	map_dimensions_ = database_->GetMapDimensions();
	// Переворот по Y
	normalizing_matrix_ = glm::scale(normalizing_matrix_, glm::vec3(1, -1, 1));
	// Сдвиг вправо вверх
	normalizing_matrix_ = glm::translate(normalizing_matrix_, glm::vec3(-0.5, -0.5, 0));
	//	Приведение масштаба к нормализованному
	normalizing_matrix_ = glm::scale(normalizing_matrix_,
					glm::vec3(1/map_dimensions_.x, 1/map_dimensions_.y, 1));

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO_passage_);
	glGenVertexArrays(1, &VAO_room_);
	return VBO && VAO_passage_&&VAO_room_;
}

void OpenGLStorage::UpdateScreenDimensions(size_t w, size_t h){
    screen_dimensions_ = Point(w,h);
    float screen_sides_ratio = ((float)w)/h;
    scaling_matrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(1, screen_sides_ratio, 1));
	updateTransformMatrix();
}

void OpenGLStorage::CommitMapMovement(int x, int y) {
	Point delta(x, -y);
	delta = delta*2;
	delta = delta/screen_dimensions_;
	moving_matrix_ = glm::translate(moving_matrix_, glm::vec3(delta.x, delta.y, 0));
	updateTransformMatrix();
	Log::info(TAG, "Go sleep 10 s");
	sleep(10);
	Log::info(TAG, "Continue");
}

void OpenGLStorage::CommitMapZoom(float d_z) {
	Log::debug(TAG, "Commit zoom");
	scaling_matrix_ = glm::scale(scaling_matrix_, glm::vec3(d_z, d_z, 1));
	updateTransformMatrix();
}

float* OpenGLStorage::GetRooms() {
	buffer_.clear();
	return getRooms();
}
float* OpenGLStorage::GetPassages() {
	buffer_.clear();
	return getPassages();
}

float* OpenGLStorage::GetObjects() {
	buffer_.clear();
	getPassages();
	float* d2 = getRooms();
	return d2;
}

const glm::f32* OpenGLStorage::GetTransformMatrix() const {
	return glm::value_ptr(result_transform_matrix_);
}

const GLuint OpenGLStorage::GetVaoRoom() const {
	return VAO_room_;
}

const GLuint OpenGLStorage::GetVaoPassage() const {
	return VAO_passage_;
}

const GLuint OpenGLStorage::GetVbo() const {
	return VBO;
}
size_t OpenGLStorage::GetVboSize() const {
	return vbo_size_;
}
void OpenGLStorage::SetVboSize(size_t size) {
	vbo_size_ = size;
}

//--------Private-----------

float* OpenGLStorage::getRooms() {
	rooms_buf_size_ = Room::GetCount() * 8;
	//rooms_buf_size_ = 8;
	buffer_.reserve(buffer_.size() + rooms_buf_size_);
	std::vector<float> verts;
	int k = 0;
	for (Room* i : room_storage_) {
		verts = i->GetVertices();
		buffer_.push_back(verts[0]);
		buffer_.push_back(verts[1]);

		buffer_.push_back(verts[2]);
		buffer_.push_back(verts[1]);

		buffer_.push_back(verts[2]);
		buffer_.push_back(verts[3]);

		buffer_.push_back(verts[0]);
		buffer_.push_back(verts[3]);
		//std::copy(verts.begin(), verts.end(), std::back_inserter(buffer_));
	}
	return &buffer_[0];
}

float* OpenGLStorage::getPassages() {
	passages_buf_size_ = Passage::GetSize();
	buffer_.reserve(buffer_.size() + passages_buf_size_);
	std::vector<float> verts;
	for (Passage* i : passage_storage_) {
		verts = i->GetVertices();
		std::copy(verts.begin(), verts.end(), std::back_inserter(buffer_));
	}
	return &buffer_[0];
}

void OpenGLStorage::updateTransformMatrix() {
	result_transform_matrix_ = moving_matrix_*scaling_matrix_*normalizing_matrix_;
}
