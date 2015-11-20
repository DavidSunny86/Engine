#version 440

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 uvCoord;

out vec3 Normal;
out vec3 DirLum;
out vec3 vecYeux;
out vec2 UV;
out vec4 shadowCoord;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 V;
uniform mat4 depthMVP;
uniform mat3 normalMatrix;
uniform vec4 lightPosition;

uniform float waveHeight;

uniform sampler2D heightMap;

void main()
{
	vec4 position = vec4(vertexPosition,1.0);
	position.y = texture(heightMap,uvCoord).r;
	gl_Position = MVP * position;
	shadowCoord = depthMVP * position;
	vec4 MVPosition = MV * position;
	DirLum =  (lightPosition.xyz - (MVPosition.xyz * lightPosition.w)).xyz;
	vecYeux = -MVPosition.xyz;
	UV = uvCoord;
}