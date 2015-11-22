#version 440

in float amplitude;
in float radius;
in vec2 centerPosition;

layout(location = 0) out float height;

uniform ivec2 heightMapSize;

float BlendFunction(float distanceFromCenter)
{
	float value = distanceFromCenter / (2 * radius);
	if(value < 0.5)
		return 1.0;
	if(value == 0.5)
		return value;
	else
		return 0.0;
}

float ComputeHeight(float distanceFromCenter)
{
	return (amplitude / 2.0) * (cos(3.14159 * distanceFromCenter /  radius) + 1.0) * BlendFunction(distanceFromCenter);
}

void main()
{
	float distanceFromCenter = 2 * radius * distance(gl_PointCoord , vec2(0.5,0.5));
	height = ComputeHeight(distanceFromCenter);
}