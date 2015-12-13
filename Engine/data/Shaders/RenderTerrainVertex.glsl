#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
out vec3 mNormal;
uniform mat4 MVP;
void main()
{
	gl_Position = MVP * position;
	mNormal = normal.xyz;
}