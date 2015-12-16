#version 440

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;

uniform sampler3D densityTexture;
uniform vec4 cubePosition;
uniform int numberOfVoxel;

uniform case_to_numberPoly_t
{
	int numberPoly[256];
};

uniform edge_connect_list_t
{
	int edgeConnect[256 * 5 * 3];
};

out struct outVertexData_s 
{
	vec4 wsPosition;
	vec4 normal;
} outVertexData;

float LinearInterpolation(in float d0, in float d1);
vec4 ComputeNormal(in vec3 wsPosition);
void main()
{
	vec4 positionInCube = gl_in[0].gl_Position;
	ivec3 voxelPosition = ivec3(positionInCube.xyz * numberOfVoxel);
	const ivec3 offset = ivec3(-1,0,1);
	float d0 = texture(densityTexture, positionInCube.xyz).r;
	float d1 = textureOffset(densityTexture, positionInCube.xyz, offset.yzy).r;
	float d2 = textureOffset(densityTexture, positionInCube.xyz, offset.zzy).r;
	float d3 = textureOffset(densityTexture, positionInCube.xyz, offset.zyy).r;
	float d4 = textureOffset(densityTexture, positionInCube.xyz, offset.yyz).r;
	float d5 = textureOffset(densityTexture, positionInCube.xyz, offset.yzz).r;
	float d6 = textureOffset(densityTexture, positionInCube.xyz, offset.zzz).r;
	float d7 = textureOffset(densityTexture, positionInCube.xyz, offset.zyz).r;

	uint voxelCase = 0;
	voxelCase |= (d0 >= 0 ? 0 : 1 << 0);
	voxelCase |= (d1 >= 0 ? 0 : 1 << 1);
	voxelCase |= (d2 >= 0 ? 0 : 1 << 2);
	voxelCase |= (d3 >= 0 ? 0 : 1 << 3);
	voxelCase |= (d4 >= 0 ? 0 : 1 << 4);
	voxelCase |= (d5 >= 0 ? 0 : 1 << 5);
	voxelCase |= (d6 >= 0 ? 0 : 1 << 6);
	voxelCase |= (d7 >= 0 ? 0 : 1 << 7); 

	for(int i = 0; i < numberPoly[voxelCase]; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			int edge = edgeConnect[voxelCase * 15 + 3 * i + j];
			float edgeLength = 1.0 / (numberOfVoxel + 1);
			float linearInterpolation;
			vec4 outPosition;
			switch(edge)
			{		
				case 0:
					linearInterpolation = LinearInterpolation(d0,d1);
					outPosition = positionInCube + vec4(0.0,linearInterpolation * edgeLength, 0.0, 0.0 ); 
					break;
				case 1:
					linearInterpolation = LinearInterpolation(d1,d2);
					outPosition = positionInCube + vec4(linearInterpolation * edgeLength, edgeLength,0.0, 0.0); 
					break;
				case 2:
					linearInterpolation = LinearInterpolation(d3,d2);
					outPosition = positionInCube + vec4(edgeLength, linearInterpolation * edgeLength, 0.0, 0.0); 
					break;
				case 3:
					linearInterpolation = LinearInterpolation(d0,d3);
					outPosition = positionInCube + vec4(linearInterpolation * edgeLength, 0.0, 0.0, 0.0); 
					break;
				case 4:
					linearInterpolation = LinearInterpolation(d4,d5);
					outPosition = positionInCube + vec4(0.0,linearInterpolation * edgeLength, edgeLength, 0.0 ); 
					break;
				case 5:
					linearInterpolation = LinearInterpolation(d5,d6);
					outPosition = positionInCube + vec4(linearInterpolation * edgeLength, edgeLength, edgeLength, 0.0); 
					break;
				case 6:
					linearInterpolation = LinearInterpolation(d7,d6);
					outPosition = positionInCube + vec4(edgeLength, linearInterpolation * edgeLength, edgeLength, 0.0);  
					break;
				case 7:
					linearInterpolation = LinearInterpolation(d4,d7);
					outPosition = positionInCube + vec4(linearInterpolation * edgeLength, 0.0, edgeLength, 0.0);
					break;
				case 8:
					linearInterpolation = LinearInterpolation(d0,d4);
					outPosition = positionInCube + vec4(0.0, 0.0, edgeLength * linearInterpolation, 0.0); 
					break;
				case 9:
					linearInterpolation = LinearInterpolation(d1,d5);
					outPosition = positionInCube + vec4(0.0, edgeLength, edgeLength * linearInterpolation, 0.0); 
					break;
				case 10:
					linearInterpolation = LinearInterpolation(d2,d6);
					outPosition = positionInCube + vec4(edgeLength, edgeLength, edgeLength * linearInterpolation, 0.0); 
					break;
				case 11:
					linearInterpolation = LinearInterpolation(d3,d7);
					outPosition = positionInCube + vec4(edgeLength, 0.0, edgeLength * linearInterpolation, 0.0); 
					break;
				default:
					outPosition = vec4(0.0,0.0,0.0,0.0);
					break;
			}

			outVertexData.normal = ComputeNormal(outPosition.xyz);
			outVertexData.wsPosition = outPosition + cubePosition;
			EmitVertex();
		}
		EndPrimitive();
	}
}


float LinearInterpolation(in float d0, in float d1)
{
	return (d0 / (d0 - d1));
}

vec4 ComputeNormal(in vec3 wsPosition)
{
	vec3 gradient;
	gradient.x = textureOffset(densityTexture,wsPosition,ivec3(1,0,0)).r - 
					textureOffset(densityTexture,wsPosition,ivec3(-1,0,0)).r;
	gradient.y = textureOffset(densityTexture,wsPosition,ivec3(0,1,0)).r - 
					textureOffset(densityTexture,wsPosition,ivec3(0,-1,0)).r + 0.001;
	gradient.z = textureOffset(densityTexture,wsPosition,ivec3(0,0,1)).r -
					textureOffset(densityTexture,wsPosition,ivec3(0,0,-1)).r;
	return vec4(normalize(-gradient),0.0);
}