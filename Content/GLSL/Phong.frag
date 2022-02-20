#version 430 core
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 ViewPos;

out vec4 color;

const uint TextureRole_Unused = 0;
const uint TextureRole_Albedo = TextureRole_Unused + 1;
const uint TextureRole_SpecularMap = TextureRole_Albedo + 1;
const uint TextureRole_BumpMap = TextureRole_SpecularMap + 1;
const uint TextureRole_GlossMap = TextureRole_BumpMap + 1;
const uint TextureRole_Mask = TextureRole_GlossMap + 1;

struct Texture
{
	uint texRole;
	sampler2D tex;
};

const int MAX_TEXTURE_COUNT = 8;
uniform Texture _Textures[MAX_TEXTURE_COUNT];
uniform int _TextureCount;

uniform float _AmbientStrength;
uniform vec3 _AmbientColor;
uniform float _SpecularStrength;
uniform vec3 _DiffColor;
uniform int _SpecularShininess;
uniform vec3 _LightColor;
uniform vec3 _LightPos;

uniform vec3 _ViewPos; // Camera view vector

void main()
{
	vec3 ambient = _AmbientStrength * _AmbientColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(_LightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * _LightColor;


	vec4 albedoCol = vec4(1.0f, 1.0f, 1.0f, -1.0f);
	vec4 specularCol = vec4(1.0f, 1.0f, 1.0f, -1.0f);

	int nbTextures = min(_TextureCount, MAX_TEXTURE_COUNT);
	for(int i = 0; i < nbTextures; i++)
	{
		uint texRole = _Textures[i].texRole;
		if(texRole == TextureRole_Unused)
		{
			continue;
		}
		else
		{
			vec4 pixel = texture(_Textures[i].tex, TexCoord);
			if(texRole == TextureRole_Albedo)
			{
				albedoCol = pixel;
			}
			else if(texRole == TextureRole_Mask)
			{
				// Determine visibility based on mask texture
				if(max(max(pixel.r, pixel.g), pixel.b) < 0.1f)
				{
					discard;
				}
			}
			else if(texRole == TextureRole_SpecularMap)
			{
				specularCol = pixel;
			}
		}
	}

	vec3 viewDir = normalize(_ViewPos - FragPos); // view direction based on current fragment and camera position
	vec3 lightReflect = reflect(-lightDir, norm); // light reflection vector
	float specAngle = max(dot(viewDir, lightReflect), 0.0f);

	vec3 specular = _LightColor * (specularCol.a > -0.5f ? specularCol.rgb : vec3(_SpecularStrength, _SpecularStrength, _SpecularStrength)) * pow(specAngle, max(1, _SpecularShininess));

	color = vec4((ambient + diffuse + specular) * _DiffColor, 1.0f) * albedoCol;
}
