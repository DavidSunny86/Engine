#include "glm/gtx/rotate_vector.hpp"
#include "WaveParticleManager.h"
#include "WaveParticle.h"
#include <iostream>
WaveParticleManager* WaveParticleManager::instance_ = NULL;

WaveParticleManager::WaveParticleManager()
	: index_(0)
	, numberOfWaveParticles_(1024 * 1024)
{
	waveParticles_.resize(numberOfWaveParticles_);
	for (int i = 0; i < numberOfWaveParticles_; ++i)
	{
		waveParticles_[i] = new WaveParticle();
	}
}

WaveParticleManager::~WaveParticleManager()
{
	for (int i = 0; i < numberOfWaveParticles_; ++i)
	{
		delete waveParticles_[i];
	}
}

void WaveParticleManager::RefreshAliveParticles()
{
    auto it = aliveParticle_.begin();
    while (it != aliveParticle_.end())
    {
        if (!(*it)->alive_)
        {
            it = aliveParticle_.erase(it);
        }
        else
        {
            it++;
        }
    }
    std::cout << aliveParticle_.size() << std::endl;
}

WaveParticle* WaveParticleManager::GetNextParticle()
{
	WaveParticle* waveParticle;
	mutex_.lock();
	{
		waveParticle = waveParticles_[index_++];
		index_ %= numberOfWaveParticles_;
        aliveParticle_.push_back(waveParticle);
	}
	mutex_.unlock();
	return waveParticle;
}

void WaveParticleManager::SpawnCircularWave(int numberOfParticles, glm::vec2 position, float amplitude, float speed, float radius)
{
	for (int i = 0; i < numberOfParticles; ++i)
	{
		WaveParticle* waveParticle = GetNextParticle();
		float dispersionAngle = 2.0f * 3.14159f / numberOfParticles;
		waveParticle->Initialize(glm::rotate(glm::vec2(1, 0), i * dispersionAngle), position, amplitude, speed, 0.1f, radius, dispersionAngle);
	}
}

WaveParticleManager* WaveParticleManager::Instance()
{
    if (instance_ == NULL)
        instance_ = new WaveParticleManager();
    return instance_;
}
