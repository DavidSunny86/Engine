#version 440

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 uvCoord;

out vec3 normal;
out vec3 modelNormal;
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

uniform vec3 scale;
uniform sampler2D heightMap;
uniform sampler2D normalMap;
void main()
{
	vec4 position = vec4(vertexPosition,1.0);
	vec3 heightMapValue = texture(heightMap,uvCoord).rgb;
	modelNormal = texture(normalMap,uvCoord).rgb;
	normal = normalMatrix * modelNormal;
	position.x += heightMapValue.g / scale.x;
	position.y = heightMapValue.r;
	position.z += heightMapValue.b / scale.z;
	gl_Position = MVP * position;
	shadowCoord = depthMVP * position;
	vec4 MVPosition = MV * position;
	DirLum =  (lightPosition.xyz - (MVPosition.xyz * lightPosition.w)).xyz;
	vecYeux = -MVPosition.xyz;
	UV = uvCoord;
}