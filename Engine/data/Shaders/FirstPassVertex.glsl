#version 440
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertexPosition;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}