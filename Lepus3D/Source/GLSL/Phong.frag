#version 330 core
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 VertColor;
in vec3 ViewPos;

out vec4 color;

uniform sampler2D _Texture1;
uniform float _AmbientStrength;
uniform float _SpecularStrength;
uniform vec3 _LightColor;
uniform vec3 _LightPos;

void main()
{
	vec3 ambient = _AmbientStrength * _LightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(_LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * _LightColor;

	vec3 viewDir = normalize(ViewPos - FragPos); // view direction based on current fragment and camera position
	vec3 lightReflect = reflect(-lightDir, norm); // light reflection vector
	float specAngle = max(dot(viewDir, lightReflect), 0.0f);
	vec3 specular = vec3(specAngle * _SpecularStrength, specAngle * _SpecularStrength, specAngle * _SpecularStrength);

	color = vec4((ambient + diffuse + specular) * VertColor, 1.0f);
}
