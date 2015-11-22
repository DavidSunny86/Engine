#pragma once
#include "glm/glm.hpp"
#include "Manager/GLSLProgramManager.h"

class WaterWaveParticles
{
public:
    WaterWaveParticles(int heightMapWidth, int heightMapHeight);
    ~WaterWaveParticles();

    void Update(double deltaT);
    GLuint GetHeightMapTexture();
protected:
    void CreateRenderBuffer();
    void CreateVertexArrayObject();
    int heightMapHeight_;
    int heightMapWidth_;

    GLSLProgram* waveProgram_;
    
    float* StartPointDirectionData_;
    float* SpeedTimeAmplitudeRadiusData_;
    
    GLuint WaveParticleFbo_;
    GLuint heightMapTexture_;
    GLuint waveStartPointDirectionTexture_;
    GLuint waveSpeedTimeAmplitudeRadiusTexture_;

    GLuint vao_;
    GLuint vbo_[3];
};

