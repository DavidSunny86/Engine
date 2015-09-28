#include <GL\glew.h>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "RenderTree.h"
#include "RenderTree/Node/Human.h"
#include "RenderTree/Node/Plan.h"
#include "Manager/ModelManager.h"

RenderTree::RenderTree(AbstractNode* parent) : CompositeNode(parent)
{
    children_.push_back(new Human(this));
    Plan* p = new Plan(this);
    p->Position(glm::vec3(0,0,-20));
    children_.push_back(p);
}


RenderTree::~RenderTree()
{
}

void RenderTree::Update(double deltaT)
{
    for (auto child : children_)
    {
        child->Update(deltaT);
    }
}

void RenderTree::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement)
{
    for (auto child : children_)
    {
        child->Render(model, view, projection, environnement);
    }
}

void RenderTree::RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{
    for (auto child : children_)
    {
        child->RenderFirstPass(model, view, projection);
    }
}

void RenderTree::RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    for (auto child : children_)
    {
        child->RenderShadowMap(model, view, projection);
    }
    glCullFace(GL_BACK);
}
