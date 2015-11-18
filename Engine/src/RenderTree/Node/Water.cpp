#include "Water.h"
#include <iostream>
#include "IL/il.h"
Water::Water(AbstractNode* parent) : AbstractNode(parent)
{
    textureWidth_ = 2048;
    textureHeight_ = 2048;
    renderingWater_ = false;
    waterProgram_ = GLSLProgramManager::Instance()->GetProgram("Water");
    CreateBuffers();
}


Water::~Water()
{
    glDeleteBuffers(1, &reflectionFbo_);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer_);
    glDeleteTextures(1, &reflectionTexture_);
    glDeleteBuffers(1, &refractionFbo_);
    glDeleteRenderbuffers(1, &refractionDepthBuffer_);
    glDeleteTextures(1, &refractionTexture_);
}

void Water::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane)
{
    if (parent_ != nullptr && !renderingWater_)
    {
        renderingWater_ = true;
        glBindFramebuffer(GL_FRAMEBUFFER, reflectionFbo_);
        glViewport(0, 0, textureWidth_, textureHeight_);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_CLIP_DISTANCE0);
        glm::mat4 modelReflection = glm::mat4(1);
        ApplyReflectionTransformation(modelReflection);
        glm::vec4 clipPlane = glm::vec4(0, -1, 0, 0);
        parent_->RenderReflection(modelReflection, view, projection, environnement, clipPlane);
        
        GLenum error = glGetError();
        glBindFramebuffer(GL_FRAMEBUFFER, refractionFbo_);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        modelReflection = glm::mat4(1);
        clipPlane = glm::vec4(0, 0, 0, 0);
        parent_->Render(modelReflection, view, projection, environnement, clipPlane);
        glDisable(GL_CLIP_DISTANCE0);
        error = glGetError();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1920, 1080);
        glActiveTexture(GL_TEXTURE7);

        glUseProgram(waterProgram_->ID());
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, reflectionTexture_);
        glActiveTexture(GL_TEXTURE2);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, refractionTexture_);
        glUniform1i(waterProgram_->GetUniformLocation("reflectionTexture"), 1);
        glUniform1i(waterProgram_->GetUniformLocation("refractionTexture"), 2);
        ApplyTransformation(model);
        model_->Render(model, view, projection, environnement);
        renderingWater_ = false;
    }
}

void Water::RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane)
{

}

void Water::Update(double deltaT)
{
}

void Water::ApplyReflectionTransformation(glm::mat4& modelReflection)
{
    modelReflection = glm::translate(modelReflection, position_);
    modelReflection = glm::scale(modelReflection, glm::vec3(1, -1, 1));
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
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
}
