#version 330 core

uniform vec4 _Color; // Colour passed from L3D
out vec4 color;

void main()
{
		color = _Color;
}