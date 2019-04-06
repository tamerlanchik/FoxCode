//  Шейдер вершин треугольника
//  по умолчанию точность вычислений: highp

//  вектор 4х значений { X, Y, Z, ? }
attribute vec4 a_Position;

void main() {
    gl_Position = a_Position;
}