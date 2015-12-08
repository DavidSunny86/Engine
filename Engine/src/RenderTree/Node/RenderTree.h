#pragma once
#include "RenderTree\Node\CompositeNode.h"
#include "Input\Handler\IKeyboardKeyHandler.h"

class Skybox;

class RenderTree 
    : public CompositeNode,
    public IKeyboardKeyHandler
{
public:
    RenderTree(AbstractNode* parent);
    virtual ~RenderTree();

    virtual void Update(double deltaT);
    virtual void Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane = glm::vec4(0)) override;
    virtual void RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, glm::vec4& clipPlane = glm::vec4(0.0)) override;
    virtual void RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection) override;
    virtual void RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane, glm::mat4 shadowModel) override;
	
    virtual void HandleKeyboardKey(int key, int action, int modifier) override;
private:
    bool pause_;
    double timeFactor_;
    Skybox* skybox_;
};

