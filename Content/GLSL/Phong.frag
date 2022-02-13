#version 430 core
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 ViewPos;

out vec4 color;

uniform int _TextureCount;
uniform sampler2D _Texture1;
uniform float _AmbientStrength;
uniform vec3 _AmbientColor;
uniform float _SpecularStrength;
uniform vec3 _LightColor;
uniform vec3 _LightPos;
uniform vec3 _DiffColor;
uniform int _SpecularShininess;

void main()
{
	vec3 ambient = _AmbientStrength * _AmbientColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(_LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * _LightColor;

	vec3 viewDir = normalize(ViewPos - FragPos); // view direction based on current fragment and camera position
	vec3 lightReflect = reflect(-lightDir, norm); // light reflection vector
	float specAngle = max(dot(viewDir, lightReflect), 0.0f);
	vec3 specular = _LightColor * _SpecularStrength * pow(specAngle, _SpecularShininess);

	vec4 textureCol = texture(_Texture1, TexCoord);
	if(_TextureCount == 0)
	{
		textureCol = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	color = vec4((ambient + diffuse + specular) * _DiffColor, 1.0f) * textureCol;
}
