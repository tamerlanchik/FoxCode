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
    std::string triangle_vert_sh_src_;
    std::string triangle_frag_sh_src_;
    GLuint u_color_location_;
    GLuint a_position_location_;
    GLuint triangle_program_id;
    //  Координаты поля OpenGL — от -1 до 1
    const GLfloat vertex_data_ [6]= {-0.5, -0.2, 0.0, 0.2, 0.5, -0.2};

    void BindData();

    static const char TAG[];
    static const char triangle_vertex_shader_name_[];
    static const char triangle_fragment_shader_name_[];
};
const char MapDrawer::TAG[] = "MapDrawer";
const char MapDrawer::triangle_vertex_shader_name_[] = "vertex_shader.glsl";
const char MapDrawer::triangle_fragment_shader_name_[] = "fragment_shader.glsl";

#endif //FOXMAPNATIVE_1_MAPDRAWER_H
