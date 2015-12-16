#version 440

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;

uniform usampler3D vertexIDTexture;

uniform case_to_numberPoly_t
{
	int numberPoly[256];
};

uniform edge_connect_list_t
{
	int edgeConnect[256 * 5 * 3];
};

uniform int numberOfVoxel;

in uint v2g_z8_y8_x8_case8[];
out uint index;

int[12] edgeAxis = { 1, 0, 1, 0, 1, 0, 1, 0, 2, 2, 2, 2};
ivec3[12] edgeStart = { ivec3(0,0,0), ivec3(0,1,0), ivec3(1,0,0),
					   ivec3(0,0,0), ivec3(0,0,1), ivec3(0,1,1),
					   ivec3(1,0,1), ivec3(0,0,1), ivec3(0,0,0),
					   ivec3(0,1,0), ivec3(1,1,0), ivec3(1,0,0) };
void main()
{
	uint cubeCase = (v2g_z8_y8_x8_case8[0] & 0xFF);
	uint numberOfPoly = numberPoly[cubeCase];

	ivec3 position;
	position.x = int(v2g_z8_y8_x8_case8[0] >> 8  & 0xFF);
	position.y = int(v2g_z8_y8_x8_case8[0] >> 16 & 0xFF);
	position.z = int(v2g_z8_y8_x8_case8[0] >> 24 & 0xFF);

	numberOfPoly = max(max(position.x,position.y),position.z) >= (numberOfVoxel - 1) ? 0 : numberOfPoly;
	for(int i = 0; i < numberOfPoly; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			int edge = edgeConnect[cubeCase * 15 + 3 * i + j];
			ivec3 voxelPosition = position + edgeStart[edge];
			voxelPosition.x = voxelPosition.x * 3 + edgeAxis[edge] ;
			index = texelFetch(vertexIDTexture, voxelPosition, 0).r;
			EmitVertex();
		}
		EndPrimitive();
	}
}