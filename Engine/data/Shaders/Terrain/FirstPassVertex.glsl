#version 440
layout(location = 0) in ivec3 voxelPosition;

uniform sampler3D densityTexture;
uniform int numberOfVoxel;

out uint v2g_z8_y8_x8_case8;
void main()
{
	const ivec3 offset = ivec3(-1,0,1);
	float d0 = texelFetch(densityTexture, voxelPosition, 0).r;
	float d1 = texelFetch(densityTexture, voxelPosition + offset.yzy, 0).r;
	float d2 = texelFetch(densityTexture, voxelPosition + offset.zzy, 0).r;
	float d3 = texelFetch(densityTexture, voxelPosition + offset.zyy, 0).r;
	float d4 = texelFetch(densityTexture, voxelPosition + offset.yyz, 0).r;
	float d5 = texelFetch(densityTexture, voxelPosition + offset.yzz, 0).r;
	float d6 = texelFetch(densityTexture, voxelPosition + offset.zzz, 0).r;
	float d7 = texelFetch(densityTexture, voxelPosition + offset.zyz, 0).r;

	uint voxelCase = 0;
	voxelCase |= (d0 >= 0 ? 0 : 1 << 0);
	voxelCase |= (d1 >= 0 ? 0 : 1 << 1);
	voxelCase |= (d2 >= 0 ? 0 : 1 << 2);
	voxelCase |= (d3 >= 0 ? 0 : 1 << 3);
	voxelCase |= (d4 >= 0 ? 0 : 1 << 4);
	voxelCase |= (d5 >= 0 ? 0 : 1 << 5);
	voxelCase |= (d6 >= 0 ? 0 : 1 << 6);
	voxelCase |= (d7 >= 0 ? 0 : 1 << 7); 
	v2g_z8_y8_x8_case8 = voxelCase;
	v2g_z8_y8_x8_case8 |= voxelPosition.x << 8;
	v2g_z8_y8_x8_case8 |= voxelPosition.y << 16;
	v2g_z8_y8_x8_case8 |= voxelPosition.z << 24;
}