#version 330 core

#define M_PI 3.1415926535897932384626433832795
#define DEG2RAD M_PI/180.0

layout (location = 0) in vec3 position;

uniform mat4 PROJ;
uniform mat4 VIEW;

uniform float runningTime;

out vec3 vertColor;

void main()
{
	float fov = 110;
	float hypot = tan((180 - fov) * 0.5 * DEG2RAD);
	float far = 100.0;
	float near = 0.1;

	float angle = runningTime / 5.0;
	float c = cos(angle);
	float s = sin(angle);

	// has to be unit length
	vec3 axis = normalize(vec3(1.0, 0.0, 0.0));

	vec3 worldPos = vec3(0.0, -2.0, -4.0);

	mat4 rot;
	rot[0] = vec4(c+axis.x*axis.x*(1.0 - c), axis.y*axis.x*(1.0 - c)+axis.z*s, axis.z*axis.x*(1.0 - c)-axis.y*s, 0.0);
	rot[1] = vec4(axis.y*axis.x*(1.0 - c)-axis.z*s, c+axis.y*axis.y*(1.0 - c), axis.z*axis.y*(1.0 - c)+axis.x*s, 0.0);
	rot[2] = vec4(axis.z*axis.x*(1.0 - c)+axis.y*s, axis.z*axis.y*(1.0 - c)-axis.x*s, c+axis.z*axis.z*(1.0 - c), 0.0);
	rot[3] = vec4(0.0, 0.0, 0.0, 1.0);

	mat4 viewPos;
	viewPos[0] = vec4(1.0, 0.0, 0.0, 0.0);
	viewPos[1] = vec4(0.0, 1.0, 0.0, 0.0);
	viewPos[2] = vec4(0.0, 0.0, 1.0, 0.0);
	viewPos[3] = vec4(worldPos, 1.0);
	// multiply rot by viewPos in C++ to build lookat matrix? could probably write out the full multiplication but ehhh

	vec4 offsetPos = vec4(position, 1.0);
	//offsetPos = rot * viewPos * (offsetPos + vec4(0.0, 0.0, 0.0, 0.0));
	//offsetPos = viewPos * offsetPos;
	gl_Position = PROJ * VIEW * offsetPos;

	float normalisedIndex = mod(float(gl_VertexID), 3.0f);
	float r = step(normalisedIndex, 0.0f);
	float g = step(normalisedIndex, 1.0f);
	float b = step(normalisedIndex, 2.0f);
	vertColor = vec3(max(0.0f, r), max(0.0f, g - r), max(0.0f, b - g - r));
}
