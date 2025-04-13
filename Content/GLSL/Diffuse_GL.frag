#version 330 core
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 VertColor;

out vec4 color;

uniform sampler2D _Texture1;
uniform float _AmbientStrength;
uniform vec3 _LightColor;
uniform vec3 _LightPos;

void main()
{
	vec3 ambient = _AmbientStrength * _LightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(_LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * _LightColor;

	color = vec4((ambient + diffuse) * VertColor, 1.0f);
}
