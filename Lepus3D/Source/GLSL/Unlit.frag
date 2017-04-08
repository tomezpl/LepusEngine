#version 330 core
#extension GL_ARB_explicit_uniform_location : enable

layout (location = 1) uniform vec4 _Color; // Colour passed from L3D
out vec4 color;

void main()
{
		color = _Color;
}