#version 330

uniform mat4 VIEW;
uniform mat4 MODEL;
uniform mat4 PROJ;

layout(location = 0) in vec3 position;

void main() {
    gl_Position = PROJ * VIEW * MODEL * vec4(position, 1.0);
}
