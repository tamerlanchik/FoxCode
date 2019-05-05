#include "OpenGLStorage.h"



OpenGLStorage::OpenGLStorage() : screen_dimensions_(Point(0,0))
{
	normalizing_matrix_ = glm::mat4(1.0);
	moving_matrix_ = glm::mat4(1.0f);
	scaling_matrix_ = glm::mat4(1.0f);
	updateTransformMatrix();


	std::cout << "OpenGLStorage created!\n";
}
OpenGLStorage* OpenGLStorage::Get() {
	static OpenGLStorage instance_;
	return &instance_;
 }

bool OpenGLStorage::InflateStorage() {
	if (!MapItemStorage::InflateStorage())
		return false;
	map_dimensions_ = database_->GetMapDimensions();
	// ��������� ��� Y
	normalizing_matrix_ = glm::scale(normalizing_matrix_, glm::vec3(1, -1, 1));
	// ����� � �����
	normalizing_matrix_ = glm::translate(normalizing_matrix_, glm::vec3(-0.5, -0.5, 0));
	//	������� �� ��������� � ��������������� �������
	normalizing_matrix_ = glm::scale(normalizing_matrix_,
					glm::vec3(1/map_dimensions_.x, 1/map_dimensions_.y, 1));
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO_passage_);
	glGenVertexArrays(1, &VAO_room_);
	assert(VBO&&VAO_passage_&&VAO_room_);
	return true;
}

const glm::f32* OpenGLStorage::GetTransformMatrix() const {
	//return glm::value_ptr(result_transform_matrix_);
	return glm::value_ptr(normalizing_matrix_);
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

void OpenGLStorage::UpdateScreenDimensions(size_t w, size_t h){
    screen_dimensions_ = Point(w,h);
    float screen_sides_ratio = ((float)w)/h;
    normalizing_matrix_ = glm::scale(normalizing_matrix_, glm::vec3(1, screen_sides_ratio, 1));
}
//--------Private-----------
void OpenGLStorage::updateTransformMatrix() {
	//result_transform_matrix_ = scaling_matrix_ * moving_matrix_*normalizing_matrix_;
}
