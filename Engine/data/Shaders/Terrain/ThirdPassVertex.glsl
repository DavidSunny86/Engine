#version 440
struct VertexData_s 
{
	vec4 wsPosition;
	vec4 normal;
};

uniform sampler3D densityTexture;
uniform vec4 cubePosition;
uniform int numberOfVoxel;
uniform int edgeSize;

out VertexData_s vertexData;

layout(location = 0) in uint z8_y8_x8_null4_edgeNum4;


float LinearInterpolation(in float d0, in float d1);
vec4 ComputeNormal(in ivec3 position);
float ComputeAmbientOcclusion(in ivec3 position);
void main()
{
	ivec3 voxelPosition;
	voxelPosition.x = int((z8_y8_x8_null4_edgeNum4 >> 8) & 0xFF);
	voxelPosition.y = int((z8_y8_x8_null4_edgeNum4 >> 16) & 0xFF);
	voxelPosition.z = int((z8_y8_x8_null4_edgeNum4 >> 24) & 0xFF);
	vec4 positionInCube = vec4(vec3(voxelPosition) / numberOfVoxel,1.0);
	int edge = int(z8_y8_x8_null4_edgeNum4 & 0xF);
	const ivec3 offset = ivec3(-1,0,1);
	float d0 = texelFetch(densityTexture, voxelPosition + edgeSize.xxx, 0 ).r;
	float d1 = texelFetch(densityTexture, voxelPosition + edgeSize.xxx + offset.yzy, 0).r;
	float d3 = texelFetch(densityTexture, voxelPosition + edgeSize.xxx + offset.zyy, 0).r;
	float d4 = texelFetch(densityTexture, voxelPosition + edgeSize.xxx + offset.yyz, 0).r;
	vec4 outPosition;
	float linearInterpolation;
	float edgeLength = 1.0 / numberOfVoxel;
	switch(edge)
	{
		case 0:
			linearInterpolation = LinearInterpolation(d0,d1);
			outPosition = positionInCube + vec4(0.0,linearInterpolation * edgeLength, 0.0, 0.0 ); 
			break;
		case 3:
			linearInterpolation = LinearInterpolation(d0,d3);
			outPosition = positionInCube + vec4(linearInterpolation * edgeLength, 0.0, 0.0, 0.0); 
			break;
		case 8:
			linearInterpolation = LinearInterpolation(d0,d4);
			outPosition = positionInCube + vec4(0.0, 0.0, edgeLength * linearInterpolation, 0.0); 
			break;
		default:
			outPosition = vec4(0.0);
	}
	vertexData.normal = ComputeNormal(voxelPosition);
	vertexData.wsPosition = outPosition + cubePosition;
}

float LinearInterpolation(in float d0, in float d1)
{
	return (d0 / (d0 - d1));
}

vec4 ComputeNormal(in ivec3 position)
{
    vec3 gradient;
    gradient.x = texelFetch(densityTexture, position + edgeSize.xxx + ivec3(1,0,0), 0).r - 
 					texelFetch(densityTexture, position + edgeSize.xxx + ivec3(-1,0,0), 0).r;
 	gradient.y = texelFetch(densityTexture, position + edgeSize.xxx + ivec3(0,1,0), 0).r - 
 					texelFetch(densityTexture, position + edgeSize.xxx + ivec3(0,-1,0), 0).r;// + 0.00001;
 	gradient.z = texelFetch(densityTexture, position + edgeSize.xxx + ivec3(0,0,1), 0).r -
 					texelFetch(densityTexture, position + edgeSize.xxx + ivec3(0,0,-1), 0).r;
 	return vec4(normalize(-gradient),ComputeAmbientOcclusion(position));
}

float ComputeAmbientOcclusion(in ivec3 position)
{
    vec3[26] direction = {
        vec3(1.0,0.0,0.0), vec3(-1.0,0.0,0.0),
        vec3(0.0,1.0,0.0), vec3(0.0,-1.0,0.0),
        vec3(0.0,0.0,1.0), vec3(0.0,0.0,-1.0),
        vec3(0.707,0.707,0.0), vec3(-0.707,0.707,0.0),
        vec3(0.707,-0.707,0.0), vec3(-0.707,-0.707,0.0),
        vec3(0.707,0.0,0.707), vec3(-0.707,0.0,0.707),
        vec3(0.707,0.0,-0.707), vec3(-0.707,0.0,-0.707),
        vec3(0.0,0.707,0.707), vec3(0.0,-0.707,0.707),
        vec3(0.0,0.707,-0.707), vec3(0.0,-0.707,-0.707),
        vec3(0.578,0.578,0.578), vec3(-0.578,0.578,0.578),
        vec3(0.578,-0.578,0.578), vec3(0.578,0.578,-0.578),
        vec3(-0.578,-0.578,0.578), vec3(-0.578,0.578,-0.578),
        vec3(-0.578,-0.578,-0.578), vec3(0.578,-0.578,-0.578)
    };

    float visibility = 0;
    for(int i = 0; i < 26; i++)
    {
        vec3 dir = direction[i];
        float thisRayVisibility = 1.0;
        for(int i = 0; i < edgeSize; i++)
        {
            float density = texelFetch(densityTexture, position + edgeSize.xxx + ivec3(i * dir), 0).x;
            thisRayVisibility *= clamp(density * 1000000, 0, 1);
        }
        visibility += thisRayVisibility;
    }
    return (1 - visibility/26.0);
}