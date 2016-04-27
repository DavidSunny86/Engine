#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>

#include "WaveParticleManager.h"
#include "WaveParticleRenderer.h"
#include "WaveParticle.h"
#include "Constant.h"
#include "Engine.h"
#include "RenderTree/Scene.h"
#include "Utility/Camera/Camera.h"

std::vector<WaveParticle*>& WaveParticleManager::GetAliveParticles()
{
    if (useFirstBuffer_)
        return firstAliveParticle_;
    else
        return secondAliveParicles_;
}

WaveParticleManager::WaveParticleManager(int waterVertexWidth, int waterVertexHeight)
	: index_(0)
{
    numberOfWaveParticles_ = Constant::maxNumberOfWaveParticleWidth * Constant::maxNumberOfWaveParticleHeight;
	waveParticles_.resize(numberOfWaveParticles_);
	for (int i = 0; i < numberOfWaveParticles_; ++i)
	{
		waveParticles_[i] = new WaveParticle(this);
	}
    renderer_ = new WaveParticleRenderer(waterVertexWidth, waterVertexHeight, this);
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
    if (useFirstBuffer_)
    {
        for (unsigned int i = 0; i < firstAliveParticle_.size(); ++i)
        {
            if (firstAliveParticle_[i]->alive_)
            {
                secondAliveParicles_.push_back(firstAliveParticle_[i]);
            }
        }
        firstAliveParticle_.clear();
    }
    else
    {
        for (unsigned int i = 0; i < secondAliveParicles_.size(); ++i)
        {
            if (secondAliveParicles_[i]->alive_)
            {
                firstAliveParticle_.push_back(secondAliveParicles_[i]);
            }
        }
        secondAliveParicles_.clear();
    }
    useFirstBuffer_ = !useFirstBuffer_;
}

WaveParticle* WaveParticleManager::GetNextParticle()
{
	WaveParticle* waveParticle;
    #pragma omp critical
	{
		waveParticle = waveParticles_[index_++];
		index_ %= numberOfWaveParticles_;
        if (useFirstBuffer_)
            firstAliveParticle_.push_back(waveParticle);
        else
            secondAliveParicles_.push_back(waveParticle);
	}
	return waveParticle;
}

void WaveParticleManager::SpawnCircularWave(int numberOfParticles, glm::vec2 position, float amplitude, float speed, float radius)
{
    float dispersionAngle = 2.0f * 3.14159f / numberOfParticles;
    float initializationTime = (radius / speed);
	for (int i = 0; i < numberOfParticles; ++i)
	{
		WaveParticle* waveParticle = GetNextParticle();
        WaveParticle* waveParticle2 = GetNextParticle();
		float dispersionAngle = 2.0f * 3.14159f / numberOfParticles;
		waveParticle->Initialize(glm::rotate(glm::vec2(1.f, 0.f), i * dispersionAngle), position, amplitude, speed, initializationTime, radius, dispersionAngle);
        waveParticle2->Initialize(glm::rotate(glm::vec2(1.f, 0.f), i * dispersionAngle), position, -amplitude, speed, 0.f , radius, dispersionAngle);
    }
}

void WaveParticleManager::Update(double deltaT)
{
    renderer_->Update(deltaT);
    RefreshAliveParticles();
}

GLuint WaveParticleManager::GetHeightMapTexture()
{
    return renderer_->GetHeightMapTexture();
}

GLuint WaveParticleManager::GetNormalHeightMapTexture()
{
    return renderer_->GetNormalHeightMapTexture();
}