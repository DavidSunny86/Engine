#version 440

layout(location = 0) in vec3 vertexPosition;
uniform mat4 MVP;
uniform mat4 M;
uniform vec4 clipPlane;

void main()
{
	vec4 MPosition = M * vec4(vertexPosition,1.0);
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	gl_ClipDistance[0] = dot(MPosition,clipPlane);
}