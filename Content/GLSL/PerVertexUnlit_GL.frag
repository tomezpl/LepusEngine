#version 430 core
in vec3 vertColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D _Texture1;
uniform sampler2D _Texture2;
uniform sampler2D _Texture3;
uniform float _Time;

void main()
{
	//color = vec4(vertColor, 1.0f);
	color = texture(_Texture1, TexCoord) * vec4(vertColor, 1.0f);
}