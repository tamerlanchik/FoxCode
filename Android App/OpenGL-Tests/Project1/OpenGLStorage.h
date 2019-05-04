#pragma once
#include "MapItemsStorage.h"
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
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
public:
	static OpenGLStorage* Get();
	virtual bool InflateStorage() override;
	const glm::f32* GetTransformMatrix() const;
private:
	void updateTransformMatrix();
};

