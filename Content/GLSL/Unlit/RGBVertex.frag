#version 330 core

out vec4 color;
in vec3 vertColor;

void main()
{
		color = vec4(vertColor, 1.0f);
}