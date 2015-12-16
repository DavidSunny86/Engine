#version 440

layout(points) in;
layout(points, max_vertices = 1) out;

out uint z8_y8_x8_case8;
in uint v2g_z8_y8_x8_case8[];
void main()
{
	uint cubeCase = v2g_z8_y8_x8_case8[0] & 0xFF;
	if(cubeCase * (255 - cubeCase) > 0)
	{
		z8_y8_x8_case8 = v2g_z8_y8_x8_case8[0];
		EmitVertex();
	}
}