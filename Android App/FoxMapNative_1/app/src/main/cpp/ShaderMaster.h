//
// Created by Andrey on 06.04.2019.
//

#ifndef FOXMAPNATIVE_1_SHADERMASTER_H
#define FOXMAPNATIVE_1_SHADERMASTER_H

#include <jni.h>
#include <android/asset_manager_jni.h>
#include <GLES2/gl2.h>
#include <string>

class ShaderMaster {
public:
    static std::string GetShaderRaw(AAssetManager* manager, const char* file_name);

    static GLuint LoadShader(GLenum shader_type, const std::string& shader_raw);

    static GLuint CreateProgram(const std::string& vertex_src, const std::string& fragment_src);
    static GLuint CreateProgram(const GLuint vertex_shader_id, const GLuint fragment_shader_id);
private:
    static const char TAG[];
};
const char ShaderMaster::TAG[] = "ShaderMaster";


#endif //FOXMAPNATIVE_1_SHADERMASTER_H
