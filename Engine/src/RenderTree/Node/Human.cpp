#include "Human.h"
#include "Manager/ModelManager.h"
#include "RenderTree/Environment/Environment.h"

Human::Human(AbstractNode* parent) : AbstractNode(parent)
{
}


Human::~Human()
{
}

void Human::Update(double deltaT)
{
    //position_[0] += 1.0 * deltaT;
}

void Human::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement)
{
    ApplyTransformation(model);
    model_->Render(model, view, projection, environnement);
}

void Human::RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{
    ApplyTransformation(model);
    model_->RenderFirstPass(model, view, projection);
}

void Human::RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{
    ApplyTransformation(model);
    model_->RenderShadowMap(model, view, projection);
}
