#pragma once
#include "RenderTree\Node\AbstractNode.h"
#include "Utility\3D\Model3D.h"

class Human :
    public AbstractNode
{
public:
    Human(AbstractNode* parent);
    ~Human();

    virtual void Update(double deltaT);
    virtual void Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement);
    virtual void RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection);
    virtual void RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection);
protected:
};

