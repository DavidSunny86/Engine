#include "WaveParticle.h"
#include "Water/WaveParticleManager.h"
#include "glm/gtx/rotate_vector.hpp"


WaveParticle::WaveParticle()
	: speed_(0.f)
	, alive_(false)
	, direction_(glm::vec2(0))
	, startPoint_(glm::vec2(0))
	, amplitude_(0.f)
	, initialTime_(0.f)
	, radius_(0.f)
	, dispersionAngle_(0.f)
{
}

WaveParticle::~WaveParticle()
{
}

glm::vec2 WaveParticle::GetPosition(float totalSimulationTime)
{
	return startPoint_ + speed_ * direction_ * (totalSimulationTime - initialTime_);
}

float WaveParticle::GetHeight()
{
	return amplitude_;
}

void WaveParticle::Subdivide()
{
	WaveParticle* leftWaveParticle = WaveParticleManager::Instance()->GetNextParticle();
	WaveParticle* rightWaveParticle = WaveParticleManager::Instance()->GetNextParticle();

	dispersionAngle_ /= 6.0f;
	amplitude_ /= 3.0f;

	leftWaveParticle->Initialize(glm::rotate(direction_, dispersionAngle_), startPoint_, amplitude_, speed_, initialTime_, radius_, dispersionAngle_);
	rightWaveParticle->Initialize(glm::rotate(direction_, -dispersionAngle_), startPoint_, amplitude_, speed_, initialTime_, radius_, dispersionAngle_);
}

void WaveParticle::Initialize(glm::vec2 direction, glm::vec2 startPoint, float amplitude, float speed, float initialTime, float radius, float dispersionAngle)
{
	direction_ = direction;
	startPoint_ = startPoint;
	amplitude_ = amplitude;
	speed_ = speed;
	initialTime_ = initialTime;
	radius_ = radius;
	dispersionAngle_ = dispersionAngle;
	alive_ = true;
}
