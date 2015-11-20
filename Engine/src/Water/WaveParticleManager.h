#pragma once

#include <mutex>
#include <vector>
#include <list>

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
    std::list<WaveParticle*> aliveParticle_;

    void RefreshAliveParticles();
	WaveParticle* GetNextParticle();

	void SpawnCircularWave(int numberOfParticles, glm::vec2 position, float amplitude, float speed);
	static WaveParticleManager* Instance();

};

