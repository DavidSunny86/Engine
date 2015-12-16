#version 440

in float amplitude;
in float radius;
in vec2 direction;

layout(location = 0) out vec3 heightMap;

uniform ivec2 heightMapSize;

float BlendFunction(in float parameter)
{
	float value = parameter / (2 * radius);
	if(value < 0.5)
		return 1.0;
	if(value == 0.5)
		return value;
	else
		return 0.0;
}

float ComputeHeight(in float distanceFromCenter)
{
	return (amplitude / 2.0) * (cos(3.14159 * distanceFromCenter /  radius) + 1.0) * BlendFunction(distanceFromCenter);
}

vec2 ComputeHorizontalDisplacement(in vec2 directionFromCenter)
{
	float u = dot(directionFromCenter, direction);
	return cos(3.14159 * u) * directionFromCenter; 
}

void main()
{
	vec2 directionFromCenter = (gl_PointCoord - vec2(0.5,0.5));
	float distanceFromCenter = radius * 2 * length(directionFromCenter);

	float height = ComputeHeight(distanceFromCenter);
	vec2 horizontalDisplacement = ComputeHorizontalDisplacement(directionFromCenter) * height;
	
	heightMap = vec3(height,horizontalDisplacement);
}