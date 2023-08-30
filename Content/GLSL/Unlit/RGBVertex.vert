#version 330 core

#define M_PI 3.1415926535897932384626433832795
#define DEG2RAD M_PI/180.0

layout (location = 0) in vec3 position;

uniform mat4 PROJ;
uniform float runningTime;

out vec3 vertColor;

void main()
{
	float fov = 110;
	float hypot = tan((180 - fov) * 0.5 * DEG2RAD);
	float far = 100.0;
	float near = 0.1;

	/*mat3 rot;*/
	float angle = runningTime;
	float c = cos(angle);
	float s = sin(angle);

	vec2 rotated = vec2(c * position.x - s * position.y, s * position.x + c * position.y);
	
	// has to be unit length
	vec3 axis = normalize(vec3(1.0, 0.0, 1.0));

	mat3 rot;
	rot[0] = vec3(c+axis.x*axis.x*(1.0 - c), axis.y*axis.x*(1.0 - c)+axis.z*s, axis.z*axis.x*(1.0 - c)-axis.y*s);
	rot[1] = vec3(axis.y*axis.x*(1.0 - c)-axis.z*s, c+axis.y*axis.y*(1.0 - c), axis.z*axis.y*(1.0 - c)+axis.x*s);
	rot[2] = vec3(axis.z*axis.x*(1.0 - c)+axis.y*s, axis.z*axis.y*(1.0 - c)-axis.x*s, c+axis.z*axis.z*(1.0 - c));

	vec3 offsetPos = rot * position;
	offsetPos = offsetPos + vec3(0.0, 0.0, -2.0);
	gl_Position = PROJ * vec4(offsetPos, 1.0);

	float normalisedIndex = mod(float(gl_VertexID), 3.0f);
	float r = step(normalisedIndex, 0.0f);
	float g = step(normalisedIndex, 1.0f);
	float b = step(normalisedIndex, 2.0f);
	vertColor = vec3(max(0.0f, r), max(0.0f, g - r), max(0.0f, b - g - r));
}
