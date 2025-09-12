#version 430 core

layout(location = 0) out vec4 color;

layout(push_constant, std430) uniform pc {
    mat4 PROJ;
    mat4 VIEW;
    mat4 MODEL;
     vec3 colour;
};

void main() {
    color = vec4(colour, 1.0);
}
