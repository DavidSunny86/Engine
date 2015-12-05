#pragma once
#include <glm/common.hpp>
#include <vector>

class GLSLProgram;
class WaveParticle;
class WaveParticleManager;

class WaveParticleRenderer
{
public:
    WaveParticleRenderer(int heightMapWidth, int heightMapHeight, WaveParticleManager* manager); 
    ~WaveParticleRenderer();

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

    WaveParticleManager* manager_;
};

