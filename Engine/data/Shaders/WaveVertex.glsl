#version 440

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 uvCoord;

uniform sampler2D startPointDirectionTexture;
uniform sampler2D speedTimeAmplitudeRadiusTexture;

out float amplitude;
out float radius;
out vec2 centerPosition;

uniform ivec2 heightMapSize;

void main()
{
	vec4 startPointDirection = texture(startPointDirectionTexture, uvCoord);
	vec4 speedTimeAmplitudeRadius = texture(speedTimeAmplitudeRadiusTexture, uvCoord);

	vec2 startPoint = startPointDirection.xy;
	vec2 direction = startPointDirection.zw;

	float speed = speedTimeAmplitudeRadius.x;
	float time = speedTimeAmplitudeRadius.y;
	amplitude = speedTimeAmplitudeRadius.z;
	radius = speedTimeAmplitudeRadius.w;

	vec2 position = startPoint + direction * speed * time;
	centerPosition = position;
	position = position * 2 - vec2(1.0,1.0);
	gl_PointSize = radius * heightMapSize.x;
	gl_Position = vec4(position,0,1);
}