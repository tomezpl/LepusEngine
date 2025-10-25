#version 330 core

uniform mat4 VIEW;
uniform mat4 MODEL;
uniform mat4 PROJ;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;

void main() {
    gl_Position = ((PROJ * VIEW * MODEL) * vec4(position, 1.0)) * vec4(1.0, -1.0, 1.0, 1.0);
    TexCoord = texCoord;
}