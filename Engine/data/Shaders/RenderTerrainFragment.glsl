#version 440
in vec3 mNormal;
out vec4 color;

void main()
{
	color = vec4(abs(mNormal),1.0);
}