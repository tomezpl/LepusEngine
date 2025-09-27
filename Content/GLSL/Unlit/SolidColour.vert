#version 430 core

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4 VIEW;
	mat4 PROJ;
} globalUbo;

layout(push_constant, std430) uniform pc
{
	mat4 PROJ;
	mat4 VIEW;
	mat4 MODEL;
vec3 colour;
} pushConstants;

layout(location = 0) in vec3 position;

void main() {
    gl_Position = vec4(position, 1.0) * pushConstants.MODEL * globalUbo.VIEW * globalUbo.PROJ;// * vec4(position, 1.0)) * vec4(1.0, -1.0, 1.0, 1.0);
}
