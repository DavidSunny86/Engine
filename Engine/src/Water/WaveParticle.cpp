#include "WaveParticle.h"
#include "Water/WaveParticleManager.h"
#include "glm/gtx/rotate_vector.hpp"


WaveParticle::WaveParticle()
    : speed_(0.f)
    , alive_(false)
    , direction_(glm::vec2(0))
    , startPoint_(glm::vec2(0))
    , amplitude_(0.f)
    , time_(0.f)
    , radius_(0.f)
    , dispersionAngle_(0.f)
    , minAmplitude_(0.1f)
{
}

WaveParticle::~WaveParticle()
{
}

glm::vec2 WaveParticle::GetPosition()
{
	return startPoint_ + speed_ * direction_ * time_;
}

float WaveParticle::GetHeight()
{
	return amplitude_;
}

void WaveParticle::Subdivide()
{
    amplitude_ /= 3.0f;
    if (amplitude_ < minAmplitude_)
    {
        alive_ = false;
        WaveParticleManager::Instance()->aliveParticle_.remove(this);
        return;
    }
    dispersionAngle_ /= 6.0f;
    
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
    int index = (position.x * width) * width + position.y * height;
    heightMap[index] += GetHeight();
}
