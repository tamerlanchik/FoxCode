#version 330 core
//  Шейдер вершин треугольника
//  по умолчанию точность вычислений: highp

layout (location = 0) in vec2 position;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}