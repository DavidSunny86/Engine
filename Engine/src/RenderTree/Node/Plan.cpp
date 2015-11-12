#include "Plan.h"
#include "Manager/ModelManager.h"


Plan::Plan(AbstractNode* parent) : AbstractNode(parent)
{
    model_ = ModelManager::Instance()->GetModel(std::string("Water"));
}


Plan::~Plan()
{
}

void Plan::Update(double deltaT)
{
}

void Plan::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement)
{
    ApplyTransformation(model);
    model_->Render(model, view, projection,environnement);
}

void Plan::RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{
    ApplyTransformation(model);
    model_->RenderFirstPass(model,view,projection);
}

void Plan::RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{
    ApplyTransformation(model);
    model_->RenderShadowMap(model, view, projection);
}