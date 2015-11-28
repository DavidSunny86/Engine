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
    GLuint GetNormalHeightMapTexture();
protected:
    void CreateRenderBuffer();
    void CreateVertexArrayObject();
    void ComputeNormalMap();
    int heightMapHeight_;
    int heightMapWidth_;

    GLSLProgram* waveProgram_;
    GLSLProgram* normalComputeProgram_;

    float* StartPointDirectionData_;
    float* SpeedTimeAmplitudeRadiusData_;
    
    GLuint WaveParticleFbo_;
    GLuint heightMapTexture_;
    GLuint normalHeightMapTexture_;
    GLuint waveStartPointDirectionTexture_;
    GLuint waveSpeedTimeAmplitudeRadiusTexture_;

    GLuint vao_;
    GLuint vbo_[3];


    //Uniform
    GLint uniformStartPointDirectionTexture_;
    GLint uniformSpeedTimeAmplitudeRadiusTexture_;
    GLint uniformHeightMapSize_;
};

