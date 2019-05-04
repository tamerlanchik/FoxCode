#pragma once
#include "MapItemsStorage.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class OpenGLStorage : public MapItemStorage
{
protected:
	OpenGLStorage();
	Point map_dimensions_;
	glm::mat4 normalizing_matrix_;
	glm::mat4 moving_matrix_;
	glm::mat4 scaling_matrix_;
	glm::mat4 result_transform_matrix_;
	size_t vbo_size_;
	GLuint VBO, VAO;
public:
	static OpenGLStorage* Get();
	virtual bool InflateStorage() override;
	const glm::f32* GetTransformMatrix() const;
	const GLuint GetVao() const;
	const GLuint GetVbo() const;
	size_t GetVboSize() const;
private:
	void updateTransformMatrix();
};

