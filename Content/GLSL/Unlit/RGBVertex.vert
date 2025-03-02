#version 430 core

layout (location = 0) in vec3 position;

layout(push_constant, std430) uniform pc
{
	mat4 PROJ;
	mat4 VIEW;
	mat4 MODEL;
};

layout(location = 0) out vec3 vertColor;

void main()
{
	gl_Position = vec4(position, 1.0) * MODEL * VIEW * PROJ;

	float normalisedIndex = mod(float(gl_VertexIndex), 3.0f);
	float r = step(normalisedIndex, 0.0f);
	float g = step(normalisedIndex, 1.0f);
	float b = step(normalisedIndex, 2.0f);
	vertColor = vec3(max(0.0f, r), max(0.0f, g - r), max(0.0f, b - g - r));
}
