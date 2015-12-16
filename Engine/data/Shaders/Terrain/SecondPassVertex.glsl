#version 440

layout(location = 0) in uint z8_y8_x8_null4_edge4;

out uint z8_y8_x8_null5_edgeFlags3;

void main()
{
	int cubeCase = int(z8_y8_x8_null4_edge4) & 0xFF;
	int bit0 = (cubeCase >> 0) & 1;
	int bit3 = (cubeCase >> 3) & 1;
	int bit1 = (cubeCase >> 1) & 1;
	int bit4 = (cubeCase >> 4) & 1;
	ivec3 onEdge = abs(ivec3(bit3,bit1,bit4) - bit0);

	uint bits = z8_y8_x8_null4_edge4 & 0xFFFFFF00;
	
	if (onEdge.x != 0)
		bits |= 1;
	if (onEdge.y != 0)
		bits |= 2;
	if (onEdge.z != 0)
		bits |= 4;

	z8_y8_x8_null5_edgeFlags3 = bits;
}