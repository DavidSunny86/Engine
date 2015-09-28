#version 440

layout(location = 0) out float fragmentDepth;
out vec4 color;
void main()
{
	fragmentDepth = gl_FragCoord.z;
}