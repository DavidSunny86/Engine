#version 440

uniform sampler2D StartPoint_Direction_Texture;	//xy = startPoint.xy   zw = direction.xy
uniform sampler2D Speed_Time_Amplitude_Radius_Texture; //x = speed y = Time z = Amplitude w = radius

uniform ivec2 HeightMapSize;

layout(r32f) uniform image2D heightMap;
layout (local_size_x = 1) in;



vec2 ComputePosition(vec2 startPoint, vec2 direction, float speed, float time)
{
	return startPoint + direction * speed * time;
}

float BlendFunction(float distanceFromCenter, float radius)
{
	float value = abs(distanceFromCenter / (2 * radius));
	if(value < 0.5)
		return 1.0;
	if(value == 0.5)
		return value;
	else
		return 0.0;
}

float ComputeHeight(vec2 center, vec2 position, float amplitude, float radius)
{
	float distanceFromCenter = length(position - center);
	return (amplitude / 2.0) * (cos(3.14159 * distanceFromCenter / radius) + 1.0) * BlendFunction(distanceFromCenter,radius);
}

void main()
{
	ivec2 particleToCompute = ivec2(gl_GlobalInvocationID.xy);
	vec4 startPointDirection = texelFetch(StartPoint_Direction_Texture, particleToCompute,0);
	vec4 speedTimeAmplitudeRadius = texelFetch(Speed_Time_Amplitude_Radius_Texture,particleToCompute,0);

	vec2 startPoint = startPointDirection.xy;
	vec2 direction = startPointDirection.zw;
	float speed = speedTimeAmplitudeRadius.x;
	float time = speedTimeAmplitudeRadius.y;
	float amplitude = speedTimeAmplitudeRadius.z;
	float radius = speedTimeAmplitudeRadius.w;

	vec2 position = ComputePosition(startPoint, direction, speed, time);

	int indexX = int(position.x * HeightMapSize.x);
    int indexY = int(position.y * HeightMapSize.y);


    int indexXMax = indexX + int(radius * HeightMapSize.x);
    int indexXMin = indexX - int(radius * HeightMapSize.x);
    int indexYMax = indexY + int(radius * HeightMapSize.y);
    int indexYMin = indexY - int(radius * HeightMapSize.y);
    float inv_width = 1.f / HeightMapSize.x;
	float inv_height = 1.f / HeightMapSize.y;
    for (int i = indexXMin; i < indexXMax; ++i)
    {
        for (int j = indexYMin; j < indexYMax; j++)
        {
            if (i >= HeightMapSize.x || j >= HeightMapSize.y || i < 0 || j < 0)
            {
                continue;
            }
            float height = ComputeHeight(position,vec2(float(i) / HeightMapSize.x, float(j) / HeightMapSize.y), amplitude, radius);
			height += imageLoad(heightMap,ivec2(i,j)).r;
			imageStore(heightMap, ivec2(i,j),vec4(height,0,0,0));
        }
	}
}