//  Шейдер точек треугольника
//  Вызывается для каждой точки фигуры

//  выставляем точность вычислений
//  { lowp, mediump, highp }
precision mediump float;
//  вектор 4х значений [ R, G, B, A ] цвета точки
//  uniform - значение одинаково для всех точек
uniform vec4 u_Color;

void main() {
    gl_FragColor = u_Color;
}