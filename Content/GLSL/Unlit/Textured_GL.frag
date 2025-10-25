#version 330 core

uniform sampler2D MyTexture;

in vec2 TexCoord;

out vec4 color;

void main() {
    color = texture(MyTexture, TexCoord);
}