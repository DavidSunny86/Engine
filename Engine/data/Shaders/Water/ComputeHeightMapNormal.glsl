#version 440

uniform sampler2D heightMap;

uniform float inv_textureWidth;
uniform vec3 scale;

layout(rgba32f) uniform writeonly image2D normalMap;

layout (local_size_x = 1, local_size_y = 1 ) in;
void main()
{
	const ivec3 offset = ivec3(-1,0,1);
	ivec2 UV = ivec2(gl_WorkGroupID.xy);
	float h[8];
	//Sobel filter
	//Position		X			Z
	// 7 6 5	1  0 -1		 1  2  1
	// 4 - 3	2  0 -2		 0  0  0
	// 2 1 0	1  0 -1		-1 -2 -1
	h[0] = texelFetch(heightMap, UV + offset.xx, 0).r;
	h[1] = texelFetch(heightMap, UV + offset.yx, 0).r;
	h[2] = texelFetch(heightMap, UV + offset.zx, 0).r;
	h[3] = texelFetch(heightMap, UV + offset.xy, 0).r;
	h[4] = texelFetch(heightMap, UV + offset.zy, 0).r;
	h[5] = texelFetch(heightMap, UV + offset.xz, 0).r;
	h[6] = texelFetch(heightMap, UV + offset.yz, 0).r;
	h[7] = texelFetch(heightMap, UV + offset.zz, 0).r;
	float sobelX = -h[0] + h[2] + 2 * (-h[3] + h[4]) - h[5] + h[7];
	float sobelZ = -h[0] + h[5] + 2 * (-h[1] + h[6]) - h[2] + h[7];
	vec3 normal = vec3(sobelX/scale.x, 8.0 * inv_textureWidth / scale.y, sobelZ/scale.z);
	normal = normalize(normal);
	imageStore(normalMap, UV, vec4(normal,0.0));
}
