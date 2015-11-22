#include "WaterWaveParticles.h"
#include "WaveParticleManager.h"
#include "WaveParticle.h"
#include <glm/gtc/type_ptr.hpp>
#include "Constant.h"

WaterWaveParticles::WaterWaveParticles(int heightMapWidth, int heightMapHeight) 
    : heightMapWidth_(heightMapWidth)
    , heightMapHeight_(heightMapHeight)
{
    StartPointDirectionData_ = new float[Constant::maxNumberOfWaveParticleWidth * Constant::maxNumberOfWaveParticleHeight * 4];
    SpeedTimeAmplitudeRadiusData_ = new float[Constant::maxNumberOfWaveParticleWidth * Constant::maxNumberOfWaveParticleHeight * 4];
    waveProgram_ = GLSLProgramManager::Instance()->GetProgram("WaveParticle");
    CreateRenderBuffer();
    CreateVertexArrayObject();
}

WaterWaveParticles::~WaterWaveParticles()
{
    delete[] StartPointDirectionData_;
    delete[] SpeedTimeAmplitudeRadiusData_;
}

void WaterWaveParticles::Update(double deltaT)
{
    GLenum error = glGetError();
    auto aliveParticles = WaveParticleManager::Instance()->GetAliveParticles();
    if (aliveParticles.size() > 0)
    {
        for (auto particule : aliveParticles)
        {
            particule->Update((float)deltaT, NULL, heightMapWidth_, heightMapHeight_);
        }
        for (int i = 0; i < (int)aliveParticles.size(); ++i)
        {
            StartPointDirectionData_[i * 4 + 0] = aliveParticles[i]->startPoint_.x;
            StartPointDirectionData_[i * 4 + 1] = aliveParticles[i]->startPoint_.y;
            StartPointDirectionData_[i * 4 + 2] = aliveParticles[i]->direction_.x;
            StartPointDirectionData_[i * 4 + 3] = aliveParticles[i]->direction_.y;

            SpeedTimeAmplitudeRadiusData_[i * 4 + 0] = aliveParticles[i]->speed_;
            SpeedTimeAmplitudeRadiusData_[i * 4 + 1] = aliveParticles[i]->time_;
            SpeedTimeAmplitudeRadiusData_[i * 4 + 2] = aliveParticles[i]->amplitude_;
            SpeedTimeAmplitudeRadiusData_[i * 4 + 3] = aliveParticles[i]->radius_;
        }
        int width = Constant::maxNumberOfWaveParticleWidth;
        int height = (aliveParticles.size() / Constant::maxNumberOfWaveParticleWidth) + 1;
        glBindTexture(GL_TEXTURE_2D, waveStartPointDirectionTexture_);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, StartPointDirectionData_);
        error = glGetError();
        glBindTexture(GL_TEXTURE_2D, waveSpeedTimeAmplitudeRadiusTexture_);
        glTexSubImage2D(GL_TEXTURE_2D, 0,0,0, width, height, GL_RGBA, GL_FLOAT, SpeedTimeAmplitudeRadiusData_);
        error = glGetError();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, WaveParticleFbo_);
    glViewport(0, 0, heightMapWidth_, heightMapHeight_);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(waveProgram_->ID());
    error = glGetError();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waveStartPointDirectionTexture_);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, waveSpeedTimeAmplitudeRadiusTexture_);
    error = glGetError();
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_ONE, GL_ONE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    error = glGetError();
    glUniform1i(waveProgram_->GetUniformLocation("startPointDirectionTexture"), 0);
    glUniform1i(waveProgram_->GetUniformLocation("speedTimeAmplitudeRadiusTexture"), 1);
    glUniform2iv(waveProgram_->GetUniformLocation("heightMapSize"), 1, glm::value_ptr(glm::ivec2(heightMapWidth_, heightMapHeight_)));
    glBindVertexArray(vao_);
    glDrawElements(GL_POINTS, aliveParticles.size(), GL_UNSIGNED_INT, 0);
    error = glGetError();
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Constant::ViewportWidth, Constant::ViewPortHeight);
    error = glGetError();
}

GLuint WaterWaveParticles::GetHeightMapTexture()
{
    return heightMapTexture_;
}

void WaterWaveParticles::CreateRenderBuffer()
{
    glGenFramebuffers(1, &WaveParticleFbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, WaveParticleFbo_);

    glGenTextures(1, &heightMapTexture_);
    glBindTexture(GL_TEXTURE_2D, heightMapTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, heightMapWidth_, heightMapHeight_, 0, GL_RED, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, heightMapTexture_, 0);

    GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw new std::exception("Water wave refraction fbo problem");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenTextures(1, &waveStartPointDirectionTexture_);
    glBindTexture(GL_TEXTURE_2D, waveStartPointDirectionTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Constant::maxNumberOfWaveParticleWidth, Constant::maxNumberOfWaveParticleHeight, 0, GL_RGBA, GL_FLOAT, StartPointDirectionData_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenTextures(1, &waveSpeedTimeAmplitudeRadiusTexture_);
    glBindTexture(GL_TEXTURE_2D, waveSpeedTimeAmplitudeRadiusTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Constant::maxNumberOfWaveParticleWidth, Constant::maxNumberOfWaveParticleHeight, 0, GL_RGBA, GL_FLOAT, SpeedTimeAmplitudeRadiusData_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void WaterWaveParticles::CreateVertexArrayObject()
{
    const int maxNumberOfParticles = Constant::maxNumberOfWaveParticleWidth * Constant::maxNumberOfWaveParticleHeight;
    GLfloat* vertices = new GLfloat[maxNumberOfParticles * 3];
    GLuint* indexes = new GLuint[maxNumberOfParticles];
    GLfloat* uvCoord = new GLfloat[maxNumberOfParticles * 2];
    for (int i = 0; i < maxNumberOfParticles * 3; ++i)
    {
        vertices[i] = 0.f;
    }
    for (int i = 0; i < maxNumberOfParticles; ++i)
    {
        indexes[i] = i;
    }
    for (int i = 0; i < Constant::maxNumberOfWaveParticleHeight; ++i)
    {
        for (int j = 0; j < Constant::maxNumberOfWaveParticleWidth; ++j)
        {
            uvCoord[(i * Constant::maxNumberOfWaveParticleHeight + j) * 2 + 0] = ((float)j / (float)Constant::maxNumberOfWaveParticleWidth);
            uvCoord[(i * Constant::maxNumberOfWaveParticleHeight + j) * 2 + 1] = ((float)i / (float)Constant::maxNumberOfWaveParticleHeight);
        }
    }

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(3, &vbo_[0]);

    //Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * maxNumberOfParticles * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    //Faces
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxNumberOfParticles * sizeof(GLuint), indexes, GL_STATIC_DRAW);

    //UVCoord
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[2]);
    glBufferData(GL_ARRAY_BUFFER, 2  * maxNumberOfParticles * sizeof(GLfloat), uvCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    delete[] vertices;
    delete[] indexes;
    delete[] uvCoord;
}
