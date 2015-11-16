#pragma once
#include "RenderTree\Node\AbstractNode.h"
class Water :
    public AbstractNode
{
public:
    Water(AbstractNode* parent);
    virtual ~Water();

    virtual void Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement);
    virtual void RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection);
    virtual void RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection);
    virtual void Update(double deltaT);

    void ApplyReflectionTransformation(glm::mat4& modelReflection);
private:
    void CreateBuffer();
    GLuint fbo_;
    GLuint reflectionTexture_;
    GLuint depthBuffer_;
    bool isRenderingReflection_;
    GLuint width_;
    GLuint height_;
};

