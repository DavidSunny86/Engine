#include "Water.h"
#include <iostream>
#include "IL/il.h"
Water::Water(AbstractNode* parent) : AbstractNode(parent)
{
    width_ = 2048;
    height_ = 2048;
    CreateBuffer();
}


Water::~Water()
{
}

void Water::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement)
{
    if (parent_ != nullptr)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        glViewport(0, 0, width_, height_);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glm::mat4 projectionReflection = glm::perspective<float>(45.f, 1.00001f, 1.f,1000.f);
        glm::mat4 modelReflection = model;
        ApplyReflectionTransformation(modelReflection);
        glEnable(GL_CLIP_DISTANCE0);
        glm::vec4 clipPlane = glm::vec4(0, 0, 0, 0);
        parent_->RenderReflection(modelReflection, view, projection, environnement, clipPlane);
        glDisable(GL_CLIP_DISTANCE0);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1920, 1080);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, reflectionTexture_);
        ApplyTransformation(model);
        model_->Render(model, view, projection, environnement);
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
    glm::mat4 transformation = glm::mat4(1);
    transformation = glm::translate(transformation, position_);
    transformation = glm::scale(transformation, glm::vec3(-1, -1, 1));
    modelReflection = transformation * modelReflection;
}

void Water::CreateBuffer()
{
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    glGenTextures(1, &reflectionTexture_);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflectionTexture_, 0);
    
    glGenRenderbuffers(1, &depthBuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer_);


    GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw new std::exception("Water fbo problem");
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
