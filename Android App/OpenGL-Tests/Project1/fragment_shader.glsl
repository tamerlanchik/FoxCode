#version 330 core
//  Шейдер точек треугольника
//  Вызывается для каждой точки фигуры
out vec4 color;
//  выставляем точность вычислений
//  { lowp, mediump, highp }
precision mediump float;
//  вектор 4х значений [ R, G, B, A ] цвета точки
//  uniform - значение одинаково для всех точек
uniform vec3 u_color;

void main() {
    //gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    color = vec4(u_color, 1.0f);
}