#version 430 core

layout(location = 0) out vec4 color;
layout(location = 0) in vec3 vertColor;

void main()
{
		color = vec4(vertColor, 1.0f);
}