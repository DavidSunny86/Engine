#include "WaveParticle.h"
#include "Water/WaveParticleManager.h"
#include "glm/gtx/rotate_vector.hpp"


WaveParticle::WaveParticle()
    : speed_(0.f)
    , direction_(glm::vec2(0))
    , startPoint_(glm::vec2(0))
    , amplitude_(0.f)
    , time_(0.f)
    , radius_(0.f)
    , dispersionAngle_(0.f)
    , minAmplitude_(0.001f)
    , alive_(false)
{
}

WaveParticle::~WaveParticle()
{
}

glm::vec2 WaveParticle::GetPosition()
{
	return startPoint_ + speed_ * direction_ * time_;
}

float WaveParticle::GetHeight(const glm::vec2& center, const glm::vec2& position)
{
    float distance = glm::length(position - center);
	return amplitude_ / 2.f * (cos(3.14159f * distance / radius_)  + 1.f) * BlendFuction(distance);
}

float WaveParticle::BlendFuction(float position)
{
    float value = abs(position / (2 * radius_));
    if (value < 0.5)
        return 1.f;
    if (value == 0.5)
        return value;
    else
        return 0.f;
}

float WaveParticle::FindTimeSinceCollision(float position, float speed, float startPoint)
{
    return time_ - (position - startPoint) / speed;
}

void WaveParticle::Subdivide()
{
    amplitude_ /= 3.0f;
    dispersionAngle_ /= 3.0f;
    
    WaveParticle* leftWaveParticle = WaveParticleManager::Instance()->GetNextParticle();
	WaveParticle* rightWaveParticle = WaveParticleManager::Instance()->GetNextParticle();

	leftWaveParticle->Initialize(glm::rotate(direction_, dispersionAngle_), startPoint_, amplitude_, speed_, time_, radius_, dispersionAngle_);
	rightWaveParticle->Initialize(glm::rotate(direction_, -dispersionAngle_), startPoint_, amplitude_, speed_, time_, radius_, dispersionAngle_);
}

void WaveParticle::Initialize(glm::vec2 direction, glm::vec2 startPoint, float amplitude, float speed, float time, float radius, float dispersionAngle)
{
	direction_ = direction;
	startPoint_ = startPoint;
	amplitude_ = amplitude;
	speed_ = speed;
	time_ = time;
	radius_ = radius;
	dispersionAngle_ = dispersionAngle;
    alive_ = true;
}

void WaveParticle::Update(float deltaT, float* heightMap, int width, int height)
{
    time_ += deltaT;
    amplitude_ -= amplitude_ * 0.05 * deltaT;
    if (amplitude_ < minAmplitude_)
    {
        alive_ = false;
        return;
    }
    glm::vec2 position = GetPosition();

    glm::vec2 normal;
    if (position.x >= 1.0f)
    {
        position.x = 1.0f;
        time_ = FindTimeSinceCollision(position.x, direction_.x * speed_, startPoint_.x);
        startPoint_ = position;
		normal = glm::vec2(1, 0);
		direction_ = glm::reflect(direction_, normal);
    }
	else if(position.y >= 1.0f)
    {
        position.y = 1.0f;
        time_ = FindTimeSinceCollision(position.y, direction_.y * speed_, startPoint_.y);
        startPoint_ = position;
		normal = glm::vec2(0, -1);
		direction_ = glm::reflect(direction_, normal);
    }
	else if(position.x <= 0.0f)
    {
        position.x = 0.0f;
        time_ = FindTimeSinceCollision(position.x, direction_.x * speed_, startPoint_.x);
        startPoint_ = position;
		normal = glm::vec2(-1, 0);
		direction_ = glm::reflect(direction_, normal);



    }
	else if(position.y <= 0.0f)
    {
        position.y = 0.0f;
        time_ = FindTimeSinceCollision(position.y, direction_.y * speed_, startPoint_.y);
        startPoint_ = position;
		normal = glm::vec2(0, 1);
		direction_ = glm::reflect(direction_, normal);
    }
 
    int indexX = (int)(position.x * width);
    int indexY = (int)(position.y * height);
    int indexXMax = indexX + (int)(radius_ * width);
    int indexXMin = indexX - (int)(radius_ * width);
    int indexYMax = indexY + (int)(radius_ * height);
    int indexYMin = indexY - (int)(radius_ * height);
    float inv_width = 1.f / width;
    for (int i = indexXMin; i < indexXMax; ++i)
    {
        for (int j = indexYMin; j < indexYMax; j++)
        {
            if (i >= width || j >= height || i < 0 || j < 0)
            {
                continue;
            }
            float waveHeight = GetHeight(position,glm::vec2((float)i * inv_width, (float)j * inv_width));
            #pragma omp critical
            {
                heightMap[i * width + j] += waveHeight;
            }
            
        }
    }

    float distanceBetweenNeighbor = 2.f*sin(dispersionAngle_) * speed_ * time_;
    if (distanceBetweenNeighbor > radius_ / 2.f)
    {
        Subdivide();
    }
}
