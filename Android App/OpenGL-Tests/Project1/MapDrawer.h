//
// Created by Andrey on 06.04.2019.
//
#pragma once
#ifndef FOXMAPNATIVE_1_MAPDRAWER_H
#define FOXMAPNATIVE_1_MAPDRAWER_H

#include "Visitor.h"
#include "MapItemsStorage.h"
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>



class MapDrawer : public Visitor{
public:
    MapDrawer();
    void Init();
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
    const GLfloat vertex_data_ [9]= {-1, -1, 0,-1, 1,0, 2, 0,0};

    MapItemStorage* map_items_;
	GLuint VBO;


    void BindData();

    void visit(const Room& r) override {

    }
    void visit(const Passage& r) override{

    }
    void visit(const Steps& r) override{

    }

    static const char TAG[];
    static const char triangle_vertex_shader_name_[];
    static const char triangle_fragment_shader_name_[];
};


#endif //FOXMAPNATIVE_1_MAPDRAWER_H
