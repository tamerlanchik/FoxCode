//
// Created by Andrey on 06.04.2019.
//

#ifndef FOXMAPNATIVE_1_SHADERMASTER_H
#define FOXMAPNATIVE_1_SHADERMASTER_H
#ifdef __ANDROID__
	#include <GLES3/gl3.h>
    #include <android/asset_manager.h>
#else
#define GLEW_STATIC
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif
#include <string>
#include <fstream>
#include <iostream>
#include "Log.h"
#include <assert.h>

class ShaderMaster {
private:
    static const char TAG[];
    static std::string readFile(const char* file_name);
    static void printError(GLuint item, const char* tag);
public:
    //static std::string GetShaderRaw(AAssetManager* manager, const char* file_name);
	static std::string GetShaderRaw(const char* file_name);
#ifdef __ANDROID__
    static std::string GetShaderRaw(AAssetManager*, const char* file_name);

#endif
    static GLuint LoadShader(GLenum shader_type, const std::string& shader_raw);

    static GLuint CreateProgram(const std::string& vertex_src, const std::string& fragment_src);
    static GLuint CreateProgram(const GLuint vertex_shader_id, const GLuint fragment_shader_id);
};


#endif //FOXMAPNATIVE_1_SHADERMASTER_H
