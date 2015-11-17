#include "AbstractNode.h"
#include "GL\glew.h"
#include "Manager/ModelManager.h"

AbstractNode::AbstractNode(AbstractNode* parent)
{
    parent = parent_;
    scale_ = glm::vec3(1.0, 1.0, 1.0);
}


AbstractNode::~AbstractNode()
{
    if (parent_ != nullptr)
        parent_->RemoveChild(this);
}

void AbstractNode::Update(double deltaT)
{

}

void AbstractNode::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement)
{
    ApplyTransformation(model);
    model_->Render(model, view, projection, environnement);
}

void AbstractNode::RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{
    ApplyTransformation(model);
    model_->RenderFirstPass(model, view, projection);
}

void AbstractNode::RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{
    ApplyTransformation(model);
    model_->RenderShadowMap(model, view, projection);
}

void AbstractNode::RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane)
{
    ApplyTransformation(model);
    model_->RenderReflection(model, view, projection, environnement, clipPlane);
}

void AbstractNode::ApplyTransformation(glm::mat4& model)
{
    model = glm::translate(model, position_);
    model = glm::rotate(model, rotation_[0], glm::vec3(1, 0, 0));
    model = glm::rotate(model, rotation_[2], glm::vec3(0, 0, 1));
    model = glm::rotate(model, rotation_[1], glm::vec3(0, 1, 0));
    model = glm::scale(model, scale_);
}

AbstractNode * AbstractNode::Parent() const
{
    return parent_;
}

void AbstractNode::Parent(AbstractNode* parent)
{
    parent_ = parent;
}

int AbstractNode::Depth() const
{
    return 1;
}

int AbstractNode::NumberOfChildren() const
{
    return 0;
}

void AbstractNode::AddChild(AbstractNode * child)
{
}

void AbstractNode::RemoveChild(AbstractNode * child)
{
}

void AbstractNode::Empty()
{
}

glm::vec3 & AbstractNode::Position()
{
    return position_;
}

glm::vec3 & AbstractNode::Rotation()
{
    return rotation_;
}

glm::vec3 & AbstractNode::Scale()
{
    return scale_;
}

void AbstractNode::Model(const std::string& name)
{
    model_ = ModelManager::Instance()->GetModel(name);
}

Model3D* AbstractNode::Model()
{
    return model_;
}

void AbstractNode::Position(const glm::vec3 & position)
{
    position_ = position;
}

void AbstractNode::Rotation(const glm::vec3 & rotation)
{
    rotation_ = rotation;
}

void AbstractNode::Scale(const glm::vec3 & scale)
{
    scale_ = scale;
}