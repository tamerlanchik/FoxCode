#include "OpenGLStorage.h"



OpenGLStorage::OpenGLStorage()
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
	glGenVertexArrays(1, &VAO);
	return true;
}

const glm::f32* OpenGLStorage::GetTransformMatrix() const {
	//return glm::value_ptr(result_transform_matrix_);
	return glm::value_ptr(normalizing_matrix_);
}

const GLuint OpenGLStorage::GetVao() const {
	return VAO;
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
void OpenGLStorage::updateTransformMatrix() {
	//result_transform_matrix_ = scaling_matrix_ * moving_matrix_*normalizing_matrix_;
}
