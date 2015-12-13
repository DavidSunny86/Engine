#include <glm/gtc/type_ptr.hpp>

#include "DensityTexture.h"
#include "RandomTextures.h"
#include "OpenGL/GLSLProgram.h"
#include "Manager/GLSLProgramManager.h"
#include "Constant.h"

DensityTexture::DensityTexture(const glm::vec3& position,RandomTextures* noise)
{
    computeDensityProgram_ = GLSLProgramManager::Instance()->GetProgram("ComputeDensityTerrain");
    CreateTexture();
    ComputeDensity(position, noise);
}


DensityTexture::~DensityTexture()
{
    glDeleteTextures(1, &densityTexture_);
}

GLuint DensityTexture::ID()
{
    return densityTexture_;
}

void DensityTexture::CreateTexture()
{
    glGenTextures(1, &densityTexture_);
    glBindTexture(GL_TEXTURE_3D, densityTexture_);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, Constant::numberOfVoxelPerTerrainCube + 1, Constant::numberOfVoxelPerTerrainCube + 1, Constant::numberOfVoxelPerTerrainCube + 1, 0, GL_RED, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
}

void DensityTexture::ComputeDensity(const glm::vec3& position, RandomTextures* noise)
{
    computeDensityProgram_->Activate();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, densityTexture_);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, noise->Texture(0));
    glUniform1i(computeDensityProgram_->GetUniformLocation("noiseTexture1"), 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, noise->Texture(1));
    glUniform1i(computeDensityProgram_->GetUniformLocation("noiseTexture2"), 2);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_3D, noise->Texture(2));
    glUniform1i(computeDensityProgram_->GetUniformLocation("noiseTexture3"), 3);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_3D, noise->Texture(3));
    glUniform1i(computeDensityProgram_->GetUniformLocation("noiseTexture4"), 4);
    glUniform1i(computeDensityProgram_->GetUniformLocation("numberOfVoxel"), Constant::numberOfVoxelPerTerrainCube);
    glBindImageTexture(0, densityTexture_, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_R16F);
    glUniform3fv(computeDensityProgram_->GetUniformLocation("position"), 1, glm::value_ptr(position));
    glDispatchCompute(Constant::numberOfVoxelPerTerrainCube + 1, Constant::numberOfVoxelPerTerrainCube + 1, Constant::numberOfVoxelPerTerrainCube + 1);
    glFinish();
}
