#version 440

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 uvCoord;
uniform mat4 MVP;
uniform mat4 M;
uniform vec4 clipPlane;
uniform sampler2D heightMap;
uniform vec3 scale;
void main()
{
	vec4 position = vec4(vertexPosition,1.0);
	vec3 heightMapValue = texture(heightMap,uvCoord).rgb;
	position.x += heightMapValue.g / scale.x;
	position.y = heightMapValue.r;
	position.z += heightMapValue.b / scale.z;
	vec4 MPosition = M * position;
	gl_Position = MVP * position;
	gl_ClipDistance[0] = dot(MPosition,clipPlane);
}