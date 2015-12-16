#version 440

layout(location = 0) in uint z8_y8_x8_null4_edge4;

flat out ivec2 vertexID_slice;

uniform int numberOfVoxel;
void main()
{
	int edgeNum = int(z8_y8_x8_null4_edge4 & 0xF); 
	ivec3 position;
	position.x = int((z8_y8_x8_null4_edge4 >> 8) & 0xFF);
	position.y = int((z8_y8_x8_null4_edge4 >> 16) & 0xFF);
	position.z = int((z8_y8_x8_null4_edge4 >> 24) & 0xFF);
	position.x *= 3;
	position.x += edgeNum == 3 ? 0 : 0;
	position.x += edgeNum == 0 ? 1 : 0;
	position.x += edgeNum == 8 ? 2 : 0;

	vec2 uvCoord = position.xy;
	uvCoord.x += 0.5 / (3.0 * numberOfVoxel);
	uvCoord.y += 0.5 / (1.0 * numberOfVoxel);

	gl_Position.x = ((2.0 * uvCoord.x / (3.0 * numberOfVoxel))) - 1.0;
	gl_Position.y = ((2.0 * uvCoord.y / (1.0 * numberOfVoxel))) - 1.0;
	//gl_Position.y = -gl_Position.y;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	vertexID_slice = ivec2(gl_VertexID, position.z);
}