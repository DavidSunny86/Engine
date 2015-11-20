#pragma once

#include <mutex>
#include <vector>

class WaveParticle;

class WaveParticleManager
{
private:
	const int numberOfWaveParticles_;
	static WaveParticleManager* instance_;
	std::vector<WaveParticle*> waveParticles_;
	std::mutex mutex_;
	int index_;

public:
	WaveParticleManager();
	~WaveParticleManager();

	WaveParticle* GetNextParticle();
	static WaveParticleManager* Instance();
};

