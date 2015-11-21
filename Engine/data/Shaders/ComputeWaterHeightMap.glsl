#version 440

uniform writeonly image2D outputTex;
layout (local_size_x = 1, local_size_y = 1) in;
void main()
{
	imageStore(outputTex,ivec2(gl_GlobalInvocationID.xy), vec4(gl_GlobalInvocationID.x,0.0,0.0,0.0));
}