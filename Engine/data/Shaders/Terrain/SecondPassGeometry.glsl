#version 440

layout(points) in;
layout(points, max_vertices = 3) out;

out uint z8_y8_x8_null4_edge4;

in uint z8_y8_x8_null5_edgeFlags3[];

void main()
{
	uint z8_y8_x8_null8 = z8_y8_x8_null5_edgeFlags3[0] & 0xFFFFFF00;
	if((z8_y8_x8_null5_edgeFlags3[0] & 1) != 0)
	{
		z8_y8_x8_null4_edge4 = z8_y8_x8_null8 | 3;
		EmitVertex();
	}
	if((z8_y8_x8_null5_edgeFlags3[0] & 2) != 0)
	{
		z8_y8_x8_null4_edge4 = z8_y8_x8_null8 | 0;
		EmitVertex();
	}
	if((z8_y8_x8_null5_edgeFlags3[0] & 4) != 0)
	{
		z8_y8_x8_null4_edge4 = z8_y8_x8_null8 | 8;
		EmitVertex();
	}
}