#pragma once

#include <vector>
#include <list>
#include <GL/glew.h>

#include "Input/Handler/IKeyboardKeyHandler.h"
#include "Input/Handler/IMouseButtonHandler.h"
#include "Input/Handler/IMousePositionHandler.h"

class WaveParticle;
class WaveParticleRenderer;

class WaveParticleManager : 
    public IKeyboardKeyHandler,
    public IMouseButtonHandler,
    public IMousePositionHandler
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
    
    virtual void HandleKeyboardKey(int key, int action, int modifier) override;
    virtual void HandleMouseKey(int button, int action, int modifier) override;
    virtual void HandleMousePosition(double xPos, double yPos) override;
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

