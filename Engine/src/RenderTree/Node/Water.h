#pragma once
#include "RenderTree\Node\AbstractNode.h"
class Water :
    public AbstractNode
{
public:
    Water(AbstractNode* parent);
    virtual ~Water();

    virtual void Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane = glm::vec4(0));
    virtual void RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane);
    virtual void Update(double deltaT);

    void ApplyReflectionTransformation(glm::mat4& modelReflection);

private:
    bool renderingWater_;
    void CreateBuffers();
    GLuint reflectionFbo_;
    GLuint reflectionTexture_;
    GLuint reflectionDepthBuffer_;
    
    GLuint refractionFbo_;
    GLuint refractionTexture_;
    GLuint refractionDepthBuffer_;

    GLuint textureWidth_;
    GLuint textureHeight_;
};

