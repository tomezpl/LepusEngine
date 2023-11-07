#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 PROJ;
uniform mat4 VIEW;

out vec3 vertColor;

void main()
{
	gl_Position = PROJ * VIEW * vec4(position, 1.0);

	float normalisedIndex = mod(float(gl_VertexID), 3.0f);
	float r = step(normalisedIndex, 0.0f);
	float g = step(normalisedIndex, 1.0f);
	float b = step(normalisedIndex, 2.0f);
	vertColor = vec3(max(0.0f, r), max(0.0f, g - r), max(0.0f, b - g - r));
}
