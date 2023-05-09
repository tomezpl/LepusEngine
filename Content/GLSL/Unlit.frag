#version 330 core

uniform vec4 _Color; // Colour passed from L3D
out vec4 color;
in vec3 vertColor;

void main()
{
		color = vec4(vertColor, 1.0f);
}