#version 440

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 uvCoord;

out vec3 Normal;
out vec3 DirLum;
out vec3 vecYeux;
out vec2 UV;
out vec4 shadowCoord;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform mat4 depthMVP;
uniform mat3 normalMatrix;
uniform vec4 lightPosition;
uniform vec4 clipPlane;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	shadowCoord = depthMVP * vec4(vertexPosition,1.0);
	vec4 MVPosition = MV * vec4(vertexPosition,1.0);
	vec4 MPosition = M * vec4(vertexPosition,1.0);
	Normal = normalMatrix * inNormal;
	DirLum =  (lightPosition.xyz - (vertexPosition * lightPosition.w)).xyz;
	vecYeux = -MVPosition.xyz;
	UV = uvCoord;

	gl_ClipDistance[0] = dot(MPosition,clipPlane);
}
