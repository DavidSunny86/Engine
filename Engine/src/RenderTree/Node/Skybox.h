#pragma once
#include "RenderTree\Node\AbstractNode.h"
class Skybox :
    public AbstractNode
{
public:
    Skybox(AbstractNode* parent);
    ~Skybox();

    virtual void Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane = glm::vec4(0));
    virtual void RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection);
    virtual void RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection);
    virtual void RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane);

    void LoadTextures();
protected:
    GLuint vbo_;
    GLuint vao_;
    GLuint texture_;
};

