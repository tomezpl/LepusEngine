#version 330 core
in vec3 vertColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D _Texture1;
uniform float _AmbientStrength;
uniform vec3 _LightColor;

void main()
{
	vec3 ambient = _AmbientStrength * _LightColor;
	color = vec4(vec3(ambient * vertColor), 1.0f) * texture(_Texture1, TexCoord);
}
