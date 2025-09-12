#version 430 core

layout(push_constant, std430) uniform pc
{
	mat4 PROJ;
	mat4 VIEW;
	mat4 MODEL;
vec3 colour;
};

layout(location = 0) in vec3 position;

void main() {
    gl_Position = vec4(position, 1.0) * MODEL * VIEW * PROJ;// * vec4(position, 1.0)) * vec4(1.0, -1.0, 1.0, 1.0);
}
