#pragma once

#include <vector>
#include <list>
#include <GL/glew.h>

class WaveParticle;
class WaveParticleRenderer;

class WaveParticleManager
{
public:
	WaveParticleManager(int waterVertexWidth, int waterVertexHeight);
	~WaveParticleManager();

    std::vector<WaveParticle*>& GetAliveParticles();
    bool useFirstBuffer_;
    void RefreshAliveParticles();
	WaveParticle* GetNextParticle();
	void SpawnCircularWave(int numberOfParticles, glm::vec2 position, float amplitude, float speed, float radius);
    void Update(double deltaT);

    GLuint GetHeightMapTexture();
    GLuint GetNormalHeightMapTexture();
private:
    int numberOfWaveParticles_;
    int index_;
    GLint mouseXPos_;
    GLint mouseYPos_;
    WaveParticleRenderer* renderer_;

    std::vector<WaveParticle*> waveParticles_;
    std::vector<WaveParticle*> firstAliveParticle_;
    std::vector<WaveParticle*> secondAliveParicles_;
};

