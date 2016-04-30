#pragma once

#include <GL/glew.h>
#include <glm/common.hpp>
class GLSLProgram;
class RandomTextures;

class DensityTexture
{
public:
    DensityTexture(const glm::vec3& position, RandomTextures* noise);
    ~DensityTexture();

    GLuint ID();
protected:
    void CreateTexture();
    void ComputeDensity(const glm::vec3& position, RandomTextures* noise);
    GLuint densityTexture_;
    int sizeOfTexture_;
    GLSLProgram* computeDensityProgram_;
};

