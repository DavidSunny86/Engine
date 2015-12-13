#version 440

uniform sampler3D noiseTexture1;
uniform sampler3D noiseTexture2;
uniform sampler3D noiseTexture3;
uniform sampler3D noiseTexture4;

uniform vec3 position;
layout(r16f) uniform image3D densityTexture;
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main()
{
	const float hard_floor = -2;
	ivec3 id = ivec3(gl_WorkGroupID);
	vec3 worldCoordinate = position + ((vec3(id) / 32.0));
	float density = -worldCoordinate.y;
	density += texture(noiseTexture1, worldCoordinate * 0.160 * 1.021).r * 0.32 * 1.16;
	density += texture(noiseTexture2, worldCoordinate * 0.080 * 0.985).r * 0.64 * 1.12;
	density += texture(noiseTexture3, worldCoordinate * 0.040 * 1.051).r * 1.28 * 1.08;
	density += texture(noiseTexture4, worldCoordinate * 0.020 * 1.023).r * 2.56 * 1.04;
	density += texture(noiseTexture4, worldCoordinate * 0.010 * 0.943).r * 5.12 * 1.0;
	//density += texture(noiseTexture4, worldCoordinate * 0.050 * 0.989).r * 10.24 * 0.96;
	density += clamp((hard_floor - worldCoordinate.y) * 3, 0, 1) * 40;
	imageStore(densityTexture, id, vec4(density,0,0,0)); 
}