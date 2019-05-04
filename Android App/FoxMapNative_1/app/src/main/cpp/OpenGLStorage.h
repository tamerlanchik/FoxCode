#pragma once
#include "MapItemsStorage.h"
#ifdef __ANDROID__
    #include <dlfcn.h>
    #include <GLES2/gl2.h>
#else
#define GLEW_STATIC
    #include <GL/glew.h>
#endif
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
	GLuint VBO, VAO;
	size_t vbo_size_;
public:
	static OpenGLStorage* Get();
	virtual bool InflateStorage() override;
	const glm::f32* GetTransformMatrix() const;
	const GLuint GetVao() const;
	const GLuint GetVbo() const;
	size_t GetVboSize() const;
	void SetVboSize(size_t);
private:
	void updateTransformMatrix();
};

