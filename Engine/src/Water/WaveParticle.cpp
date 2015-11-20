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
    , minAmplitude_(0.00f)
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

float WaveParticle::GetHeight(const glm::vec2& position)
{
    float distance = position.length() - GetPosition().length();
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

void WaveParticle::Subdivide()
{
    amplitude_ /= 3.0f;
    if (amplitude_ < minAmplitude_)
    {
        alive_ = false;
        return;
    }
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
    float distanceBetweenNeighbor = dispersionAngle_ * speed_ * time_;
    if (distanceBetweenNeighbor > radius_ / 2.f)
        Subdivide();

    glm::vec2 position = GetPosition();
    int indexX = position.x * width;
    int indexY = position.y * height;
    if (indexX >= width || indexY >= height || indexX < 0 || indexY < 0)
    {
        alive_ = false;
        return;
    }
    int indexXMax = indexX + radius_ * width;
    int indexXMin = indexX - radius_ * width;
    int indexYMax = indexY + radius_ * height;
    int indexYMin = indexY - radius_ * height;
    for (int i = indexXMin; i < indexXMax; ++i)
    {
        for (int j = indexYMin; j < indexYMax; j++)
        {
            if (i >= width || j >= height || i < 0 || j < 0)
            {
                continue;
            }
            heightMap[i * width + j] += GetHeight(glm::vec2(i/width,j/height));
        }
    }
}
