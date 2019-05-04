#version 300 es
precision mediump float;
out vec4 color;
uniform vec3 u_color;

void main() {
    //gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    color = vec4(u_color, 1.0f);
}