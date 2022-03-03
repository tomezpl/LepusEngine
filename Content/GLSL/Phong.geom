#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


struct VERTEXFORMAT {
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
	vec3 VertPos;
	vec3 Tangent;
	vec3 Bitangent;
};

in VERTEXFORMAT vs_out[];
out VERTEXFORMAT fs_in;

void CalculateTangent(int n)
{
	int v1 = -1;
	int v2 = -1;
	for(int i = 0; i < 3; i++)
	{
		if(n != i)
		{
			if(v1 == -1)
			{
				v1 = i;
			}
			if(v2 == -1)
			{
				v2 = i;
			}
		}
	}

	vec2 uv1 = vs_out[v1].TexCoord;
	vec2 uv2 = vs_out[v2].TexCoord;

	mat2 uvMat = mat2(
		uv1.x, uv2.x,
		uv1.y, uv2.y);

	vec3 e1 = vs_out[v1].VertPos - vs_out[n].VertPos;
	vec3 e2 = vs_out[v2].VertPos - vs_out[n].VertPos;

	mat3x2 eMat = mat3x2(e1.x, e2.x, e1.y, e2.y, e1.z, e2.z);

	mat3x2 tbMat = inverse(uvMat) * eMat;

	vec3 t = vec3(tbMat[0][0], tbMat[1][0], tbMat[2][0]);
	vec3 b = vec3(tbMat[0][1], tbMat[1][1], tbMat[2][1]);

	fs_in.Tangent = t;
	fs_in.Bitangent = b;
}

void main()
{
	gl_Position = gl_in[0].gl_Position;
	fs_in = vs_out[0];
	CalculateTangent(0);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	fs_in = vs_out[1];
	CalculateTangent(1);
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	fs_in = vs_out[2];
	CalculateTangent(2);
	EmitVertex();

	EndPrimitive();
}