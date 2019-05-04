#pragma once
#ifdef __ANDROID__
	#include <dlfcn.h>
	#include <GLES3/gl3.h>
#else
#define GLEW_STATIC
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif
#include <string>
#include "ShaderMaster.h"
#include <assert.h>

class ShaderProgram
{
private:
	GLuint program_id_;
	GLuint transform_param_location_;
	GLuint vertex_color_param_location_;
	std::string vertex, frag;
	static const char border_color_key[];
	static const char transform_matrix_key[];
public:
	ShaderProgram() {};
	ShaderProgram(const char* vertex_shader_name, const char* fragment_shader_name);

#ifdef __ANDROID__
	ShaderProgram(AAssetManager*, const char*, const char*);
#endif
	~ShaderProgram();
	GLuint GetTransformParamLocation() const;
	GLuint GetColorParamLocation() const;
	void Use();
	void SetTransformMatrix(const GLfloat* ptr);
	void SetVertexColor(const float r, const float g, const float b);
	void Generate();
};

