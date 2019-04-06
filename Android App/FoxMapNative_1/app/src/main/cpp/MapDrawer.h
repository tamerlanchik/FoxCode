//
// Created by Andrey on 06.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPDRAWER_H
#define FOXMAPNATIVE_1_MAPDRAWER_H

#include <jni.h>
#include <cstdlib>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <string>
#include <GLES2/gl2.h>


class MapDrawer{
public:
    MapDrawer() {};
    void Init( AAssetManager* );
    void Render();
    void SurfaceChanged(int w, int h);
    void SurfaceCreated();
private:
    AAssetManager* assert_manager_;
    std::string triangle_vert_sh_src;
    std::string triangle_frag_sh_src;

    std::string GetVertexShader(AAssetManager* manager, const char*);
    std::string GetFragmentShader(AAssetManager* manager, const char*);

    GLuint LoadShader(GLenum shader_type, const std::string& shader_raw);

    GLuint CreateProgram(const char* vertex_file_name, const char* fragment_file_name);
    GLuint CreateProgram(const GLuint vertex_shader_id, const GLuint fragment_shader_id);

    static const char TAG[];
    static const char triangle_vertex_shader_name_[];
    static const char triangle_fragment_shader_name_[];
};
const char MapDrawer::TAG[] = "MapDrawer";
const char MapDrawer::triangle_vertex_shader_name_[] = "vertex_shader.glsl";
const char MapDrawer::triangle_fragment_shader_name_[] = "fragment_shader.glsl";

#endif //FOXMAPNATIVE_1_MAPDRAWER_H
