#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

struct VERTEXFORMAT {
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
};

out VERTEXFORMAT vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vs_out.Normal = mat3(transpose(inverse(model))) * normal; // apply Normal Matrix for non-uniform scaling (TODO: write as CPU task)
	vs_out.TexCoord = texCoord;
	vs_out.FragPos = vec3(model * vec4(position, 1.0f));
}
