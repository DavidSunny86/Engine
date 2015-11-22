#pragma once

#include <vector>
#include <list>

class WaveParticle;

class WaveParticleManager
{
private:
	int numberOfWaveParticles_;
	static WaveParticleManager* instance_;
	std::vector<WaveParticle*> waveParticles_;

	int index_;
    std::vector<WaveParticle*> firstAliveParticle_;
    std::vector<WaveParticle*> secondAliveParicles_;
public:
	WaveParticleManager();
	~WaveParticleManager();

    std::vector<WaveParticle*>& GetAliveParticles();
    bool useFirstBuffer_;
    void RefreshAliveParticles();
	WaveParticle* GetNextParticle();

	void SpawnCircularWave(int numberOfParticles, glm::vec2 position, float amplitude, float speed, float radius);
	static WaveParticleManager* Instance();

};

