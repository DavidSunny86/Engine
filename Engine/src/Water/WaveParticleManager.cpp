#include "WaveParticleManager.h"
#include "WaveParticle.h"

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

WaveParticleManager* WaveParticleManager::Instance()
{
    if (instance_ == NULL)
        instance_ = new WaveParticleManager();
    return instance_;
}
