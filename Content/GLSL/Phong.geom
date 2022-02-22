#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


struct VERTEXFORMAT {
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
};

in VERTEXFORMAT vs_out[];
out VERTEXFORMAT fs_in;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	fs_in = vs_out[0];
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	fs_in = vs_out[1];
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	fs_in = vs_out[2];
	EmitVertex();

	EndPrimitive();
}