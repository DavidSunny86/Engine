#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

smooth out vec3 Normal;
smooth out vec3 DirLum;
smooth out vec3 vecYeux;

uniform vec4 lightPosition;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 normalMatrix;
void main()
{
	gl_Position = MVP * position;
	Normal = normalMatrix * normal.xyz;
	vec4 MVPosition = MV * position;
	DirLum = (lightPosition.xyz - (MVPosition.xyz * lightPosition.w)).xyz;
	vecYeux = -MVPosition.xyz;
}