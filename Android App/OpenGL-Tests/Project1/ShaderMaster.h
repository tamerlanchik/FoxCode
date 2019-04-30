//
// Created by Andrey on 06.04.2019.
//

#ifndef FOXMAPNATIVE_1_SHADERMASTER_H
#define FOXMAPNATIVE_1_SHADERMASTER_H
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>

class ShaderMaster {
public:
    //static std::string GetShaderRaw(AAssetManager* manager, const char* file_name);
	static std::string GetShaderRaw(const char* file_name) {
		return readFile(file_name);
	}

    static GLuint LoadShader(GLenum shader_type, const std::string& shader_raw);

    static GLuint CreateProgram(const std::string& vertex_src, const std::string& fragment_src);
    static GLuint CreateProgram(const GLuint vertex_shader_id, const GLuint fragment_shader_id);
private:
    static const char TAG[];
	static std::string readFile(const char* file_name);
};


#endif //FOXMAPNATIVE_1_SHADERMASTER_H
