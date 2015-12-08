#include <GL\glew.h>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

#include "RenderTree.h"
#include "RenderTree/Node/Human.h"
#include "RenderTree/Node/Plan.h"
#include "RenderTree/Node/Skybox.h"
#include "Manager/ModelManager.h"
#include "RenderTree/Node/Skybox.h"

RenderTree::RenderTree(AbstractNode* parent)
    : CompositeNode(parent)
    , timeFactor_(1.0)
{
    skybox_ = new Skybox(this);
}


RenderTree::~RenderTree()
{
    delete skybox_;
}

void RenderTree::Update(double deltaT)
{
	if (!pause_)
	{
		for (auto child : children_)
		{
			child->Update(deltaT * timeFactor_);
		}
	}
}

void RenderTree::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane)
{
    for (auto child : children_)
    {
        child->Render(model, view, projection, environnement, clipPlane);
    }
    skybox_->Render(model, view, projection, environnement);
}

void RenderTree::RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, glm::vec4& clipPlane)
{
    for (auto child : children_)
    {
        child->RenderFirstPass(model, view, projection, clipPlane);
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

void RenderTree::RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane, glm::mat4 shadowModel)
{
    for (auto child : children_)
    {
        child->RenderReflection(model, view, projection,environnement,clipPlane, shadowModel);
    }
    skybox_->RenderReflection(model, view, projection, environnement, clipPlane, shadowModel);
}

void RenderTree::HandleKeyboardKey(int key, int action, int modifier)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_P:
            pause_ = !RenderTree::pause_;
            break;
        case GLFW_KEY_PAGE_UP:
            timeFactor_ += 0.05;
            break;
        case GLFW_KEY_PAGE_DOWN:
            timeFactor_ -= 0.05;
            break;
        }
    }
}
