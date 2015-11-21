#include "Water.h"
#include <iostream>
#include "IL/il.h"
#include "Constant.h"
#include "Water/WaveParticleManager.h"
#include "Water/WaveParticle.h"
#include <glm/gtc/type_ptr.hpp>


int Water::waterNumberOfVertexWidth_ = 32;
int Water::waterNumberOfVertexHeight_ = 32;
glm::vec3 Water::ambientMaterial_ = glm::vec3(0.2,0.2,0.2);
glm::vec3 Water::diffuseMaterial_ = glm::vec3(1.0,1.0,1.0);
glm::vec3 Water::specularMaterial_ = glm::vec3(1.0,1.0,1.0);
float Water::shininess_ = 100;


Water::Water(AbstractNode* parent) : AbstractNode(parent)
{
    textureWidth_ = Constant::TextureWidth;
    textureHeight_ = Constant::TextureHeight;
    renderingWater_ = false;
    waterProgram_ = GLSLProgramManager::Instance()->GetProgram("Water");
    firstPassProgram_ = GLSLProgramManager::Instance()->GetProgram("WaterFirstPass");
    heightMapData_ = new float[waterNumberOfVertexHeight_ * waterNumberOfVertexWidth_];
    waveHeight_ = 2.f;
    reflectionPerturbationFactor_ = 0.01f;
    CreateBuffers();
    LoadModel();
}


Water::~Water()
{
    glDeleteBuffers(1, &reflectionFbo_);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer_);
    glDeleteTextures(1, &reflectionTexture_);
    glDeleteBuffers(1, &refractionFbo_);
    glDeleteRenderbuffers(1, &refractionDepthBuffer_);
    glDeleteTextures(1, &refractionTexture_);
    glDeleteBuffers(3, vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void Water::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane)
{
    if (parent_ != nullptr && !renderingWater_)
    {
        renderingWater_ = true;
        RenderWaterReflection(model,view,projection,environnement);
        RenderWaterRefraction(model,view,projection,environnement);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, Constant::ViewportWidth, Constant::ViewPortHeight);
        ApplyTransformation(model);
        RenderModel(model, view, projection, environnement);
        renderingWater_ = false;
    }
}



void Water::RenderWaterReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement)
{
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFbo_);
    glViewport(0, 0, textureWidth_, textureHeight_);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_CLIP_DISTANCE0);
    glm::mat4 modelReflection = glm::mat4(1);
    ApplyReflectionTransformation(modelReflection);
    glm::vec4 clipPlane = glm::vec4(0, -1, 0, 0);
    parent_->RenderReflection(modelReflection, view, projection, environnement, clipPlane, glm::mat4(1));
}

void Water::RenderWaterRefraction(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement)
{
    glBindFramebuffer(GL_FRAMEBUFFER, refractionFbo_);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glm::mat4 modelReflection = glm::mat4(1);
    glm::vec4 clipPlane = glm::vec4(0, 1, 0, 0);
    parent_->RenderFirstPass(modelReflection, view, projection);
    parent_->Render(modelReflection, view, projection, environnement, clipPlane);
    glDisable(GL_CLIP_DISTANCE0);
}

void Water::RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane, glm::mat4 shadowModel)
{

}

void Water::RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{
    if (!renderingWater_)
    {
        ApplyTransformation(model);
        RenderModelFirstPass(model, view, projection);
    }
}

void Water::Update(double deltaT)
{
    time_ += deltaT;
    
    for (int i = 0; i < waterNumberOfVertexHeight_ * waterNumberOfVertexWidth_; ++i)
    {
        heightMapData_[i] = 0.f;
    }

    auto aliveParticles = WaveParticleManager::Instance()->GetAliveParticles();
    #pragma omp parallel for
    for (int i = 0; i < (int)aliveParticles.size(); ++i)
    {
        aliveParticles[i]->Update((float)deltaT, heightMapData_, waterNumberOfVertexWidth_, waterNumberOfVertexHeight_);
    }
    glBindTexture(GL_TEXTURE_2D, heigthMapTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, waterNumberOfVertexWidth_, waterNumberOfVertexHeight_, 0, GL_RED, GL_FLOAT, heightMapData_);
    WaveParticleManager::Instance()->RefreshAliveParticles();
}

void Water::ApplyReflectionTransformation(glm::mat4& modelReflection)
{
    modelReflection = glm::translate(modelReflection, position_);
    modelReflection = glm::scale(modelReflection, glm::vec3(1, -1, 1));
}


void Water::RenderModel(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, Environment* e, const glm::vec4& clipPlane /*= glm::vec4(0)*/)
{
    glUseProgram(waterProgram_->ID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture_);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, refractionTexture_);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, heigthMapTexture_);
    glUniform1i(waterProgram_->GetUniformLocation("reflectionTexture"), 1);
    glUniform1i(waterProgram_->GetUniformLocation("refractionTexture"), 2);
    glUniform1i(waterProgram_->GetUniformLocation("heightMap"), 3);
    glm::vec2 viewPort = glm::vec2(Constant::ViewportWidth, Constant::ViewPortHeight);
    glUniform2i(waterProgram_->GetUniformLocation("viewPort"), Constant::ViewportWidth, Constant::ViewPortHeight);
    glUniform1f(waterProgram_->GetUniformLocation("inv_textureWidth"), 1.f / waterNumberOfVertexWidth_);
    glUniform1f(waterProgram_->GetUniformLocation("reflectionPerturbationFactor"), reflectionPerturbationFactor_);
    glUniform3fv(waterProgram_->GetUniformLocation("scale"), 1,glm::value_ptr(scale_));
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(mv)));
    glUniformMatrix4fv(waterProgram_->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(waterProgram_->GetUniformLocation("MV"), 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(waterProgram_->GetUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(m));
    glUniformMatrix3fv(waterProgram_->GetUniformLocation("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    for (auto light : e->lights_)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light->shadowMap_);
        glUniform1i(waterProgram_->GetUniformLocation("shadowMap"), 0);
        glm::mat4 depthMVP = light->dephtBiasMVP_ * light->projection_ * light->view_ * m;
        glUniformMatrix4fv(waterProgram_->GetUniformLocation("depthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
        glUniform4fv(waterProgram_->GetUniformLocation("lightAmbientColor"), 1, glm::value_ptr(light->ambientColor_));
        glUniform4fv(waterProgram_->GetUniformLocation("lightDiffuseColor"), 1, glm::value_ptr(light->diffuseColor_));
        glUniform4fv(waterProgram_->GetUniformLocation("lightSpecularColor"), 1, glm::value_ptr(light->specularColor_));
        glUniform4fv(waterProgram_->GetUniformLocation("lightPosition"), 1, glm::value_ptr(light->transformedPosition_));
        glUniform1f(waterProgram_->GetUniformLocation("lightIntensity"), light->power_);

        glUniform4fv(waterProgram_->GetUniformLocation("materialAmbient"), 1, glm::value_ptr(ambientMaterial_));
        glUniform4fv(waterProgram_->GetUniformLocation("materialDiffuse"), 1, glm::value_ptr(diffuseMaterial_));
        glUniform4fv(waterProgram_->GetUniformLocation("materialSpecular"), 1, glm::value_ptr(specularMaterial_));
        glUniform1f(waterProgram_->GetUniformLocation("shininess"), shininess_);
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, numberOfFaces_ * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    glDisable(GL_BLEND);
}

void Water::RenderModelFirstPass(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, const glm::vec4& clipPlane /*= glm::vec4(0)*/)
{
    glUseProgram(firstPassProgram_->ID());
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glUniformMatrix4fv(firstPassProgram_->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(firstPassProgram_->GetUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(m));
    glUniform4fv(firstPassProgram_->GetUniformLocation("clipPlane"), 1, glm::value_ptr(clipPlane));
	glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, heigthMapTexture_);
    glUniform1i(firstPassProgram_->GetUniformLocation("heightMap"), 3);
    glDisable(GL_BLEND);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, numberOfFaces_ * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Water::LoadModel()
{
    numberOfFaces_ = (waterNumberOfVertexWidth_ - 1) * (waterNumberOfVertexHeight_ - 1) * 2;
    GLfloat* vertices = new GLfloat[waterNumberOfVertexWidth_ * waterNumberOfVertexHeight_ * 3];
    GLuint* indexes = new GLuint[numberOfFaces_ * 3];
    GLfloat* uvCoord = new GLfloat[waterNumberOfVertexWidth_ * waterNumberOfVertexHeight_ * 2];
    
    float inv_width = 1.f / (float)waterNumberOfVertexWidth_;
    float inv_heigth = 1.f / (float)waterNumberOfVertexHeight_;
    float inv_width2 = 2.f * inv_heigth;
    float inv_heigth2 = 2.f * inv_heigth;
    for (int i = 0; i < waterNumberOfVertexWidth_; ++i)
    {
        for (int j = 0; j < waterNumberOfVertexHeight_; ++j)
        {
            vertices[(i * waterNumberOfVertexWidth_ + j) * 3 + 0] = 1.f - i * inv_width2;
            vertices[(i * waterNumberOfVertexWidth_ + j) * 3 + 1] = 0;
            vertices[(i * waterNumberOfVertexWidth_ + j) * 3 + 2] = 1.f - j * inv_heigth2;
        }
    }
    for (int i = 0; i < waterNumberOfVertexWidth_ - 1; ++i)
    {
        for (int j = 0; j < waterNumberOfVertexHeight_ - 1; ++j)
        {
            indexes[(i * (waterNumberOfVertexWidth_ - 1) + j) * 6 + 0] = i * waterNumberOfVertexWidth_ + j + 0;
            indexes[(i * (waterNumberOfVertexWidth_ - 1) + j) * 6 + 1] = i * waterNumberOfVertexWidth_ + j + 1;
            indexes[(i * (waterNumberOfVertexWidth_ - 1) + j) * 6 + 2] = i * waterNumberOfVertexWidth_ + j + waterNumberOfVertexWidth_;
            indexes[(i * (waterNumberOfVertexWidth_ - 1) + j) * 6 + 3] = i * waterNumberOfVertexWidth_ + j + 1;
            indexes[(i * (waterNumberOfVertexWidth_ - 1) + j) * 6 + 4] = i * waterNumberOfVertexWidth_ + j + waterNumberOfVertexWidth_ + 1;
            indexes[(i * (waterNumberOfVertexWidth_ - 1) + j) * 6 + 5] = i * waterNumberOfVertexWidth_ + j + waterNumberOfVertexWidth_;
        }
    }
    for (int i = 0; i < waterNumberOfVertexWidth_; ++i)
    {
        for (int j = 0; j < waterNumberOfVertexHeight_; ++j)
        {
            uvCoord[(i * waterNumberOfVertexWidth_ + j) * 2 + 0] = i * inv_width;
            uvCoord[(i * waterNumberOfVertexWidth_ + j) * 2 + 1] = j * inv_heigth;
        }
    }
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    glGenBuffers(3, &vbo_[0]);

    //Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * waterNumberOfVertexWidth_ * waterNumberOfVertexHeight_ * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    //Faces
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * numberOfFaces_ * sizeof(GLuint), indexes, GL_STATIC_DRAW);

    //UVCoord
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[2]);
    glBufferData(GL_ARRAY_BUFFER, 2 * waterNumberOfVertexWidth_ * waterNumberOfVertexHeight_ * sizeof(GLfloat), uvCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    delete[] vertices;
    delete[] indexes;
    delete[] uvCoord;
}

void Water::CreateBuffers()
{
    glGenFramebuffers(1, &reflectionFbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFbo_);

    glGenTextures(1, &reflectionTexture_);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth_, textureHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflectionTexture_, 0);
    
    glGenRenderbuffers(1, &reflectionDepthBuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, reflectionDepthBuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureWidth_, textureHeight_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, reflectionDepthBuffer_);


    GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw new std::exception("Water reflection fbo problem");
    
    glGenFramebuffers(1, &refractionFbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, refractionFbo_);

    glGenTextures(1, &refractionTexture_);
    glBindTexture(GL_TEXTURE_2D, refractionTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth_, textureHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, refractionTexture_, 0);

    glGenRenderbuffers(1, &refractionDepthBuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, refractionDepthBuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureWidth_, textureHeight_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, refractionDepthBuffer_);

    glDrawBuffers(1, buffers);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw new std::exception("Water refraction fbo problem");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenTextures(1, &heigthMapTexture_);
    glBindTexture(GL_TEXTURE_2D, heigthMapTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, waterNumberOfVertexWidth_, waterNumberOfVertexHeight_, 0, GL_RED, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
