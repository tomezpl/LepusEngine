#version 330 core

#define M_PI 3.1415926535897932384626433832795
#define DEG2RAD M_PI/180.0

layout (location = 0) in vec3 position;

uniform mat4 PROJ;

out vec3 vertColor;

void main()
{
	float fov = 110;
	float hypot = tan((180 - fov) * 0.5 * DEG2RAD);
	float far = 100.0;
	float near = 0.1;

	/*mat4 PROJ;
	PROJ[0] = vec4(hypot, 0.0, 0.0, 0.0);
	PROJ[1] = vec4(0.0, hypot, 0.0, 0.0);
	PROJ[2] = vec4(0.0, 0.0, -(far / (far - near)), -((far * near) / (far - near)));
	PROJ[3] = vec4(0.0, 0.0, -1.0, 0.0);*/

	vec3 offsetPos = position + vec3(0.0, 0.0, 1.5);
	gl_Position = vec4(offsetPos, 1.0f) * PROJ;
	gl_Position.w = offsetPos.z;

	float normalisedIndex = mod(float(gl_VertexID), 3.0f);
	float r = step(normalisedIndex, 0.0f);
	float g = step(normalisedIndex, 1.0f);
	float b = step(normalisedIndex, 2.0f);
	vertColor = vec3(max(0.0f, r), max(0.0f, g - r), max(0.0f, b - g - r));
}
