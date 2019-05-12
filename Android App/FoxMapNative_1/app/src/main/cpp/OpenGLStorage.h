#pragma once
#include "MapItemsStorage.h"
#include "Point.h"
#ifdef __ANDROID__
    #include <GLES3/gl3.h>
#else
	#define GLEW_STATIC
    #include <GL/glew.h>
#endif
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <system_error>

class OpenGLStorage : public MapItemStorage
{
protected:
	OpenGLStorage();
	Point map_dimensions_;
	Point screen_dimensions_;
	glm::mat4 normalizing_matrix_;
	glm::mat4 moving_matrix_;
	glm::mat4 scaling_matrix_;
	glm::mat4 result_transform_matrix_;
	GLuint VBO, VAO_room_, VAO_passage_;
	size_t vbo_size_;
	float* getRooms();
	float* getPassages();
public:
	static OpenGLStorage* Get();
	virtual bool InflateStorage() override;
	const glm::f32* GetTransformMatrix() const;
	const GLuint GetVaoRoom() const;
	const GLuint GetVaoPassage() const;
	const GLuint GetVbo() const;
	size_t GetVboSize() const;
	void SetVboSize(size_t);
	void UpdateScreenDimensions(size_t w, size_t h);
	float* GetRooms();
	float* GetPassages();
	float* GetObjects();
	size_t GetRoomsBufSize() const { return rooms_buf_size_; }
	size_t GetPassagesBufSize() const { return passages_buf_size_; }
	const size_t GetBufferSize() const { return buffer_.size(); }

	void CommitMapMovement(int x, int y);
	void CommitMapZoom(float delta);
private:
	void updateTransformMatrix();
	static const char TAG[];
};

