#version 440

in float amplitude;
in float radius;
in vec2 centerPosition;

layout(location = 0) out vec4  height;

uniform ivec2 heightMapSize;

float BlendFunction(float distanceFromCenter, float radius)
{
	float value = distanceFromCenter / ( radius);
	if(value < 0.5)
		return 1.0;
	if(value == 0.5)
		return value;
	else
		return 0.0;
}

float ComputeHeight(vec2 position)
{
	float distanceFromCenter = length(position - centerPosition);
	return (amplitude / 2.0) * (cos(3.14159 * distanceFromCenter / radius) + 1.0) * BlendFunction(distanceFromCenter,radius);
}

void main()
{
	vec2 position = vec2(float(gl_FragCoord.x) / float(heightMapSize.x), float(gl_FragCoord.y) / float(heightMapSize.y));
	height = vec4(ComputeHeight(position),0,0,0);
}