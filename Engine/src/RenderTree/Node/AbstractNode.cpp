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
    ApplyReflectionTransformation(model);
    model_->RenderReflection(model, view, projection, environnement, clipPlane);
}

void AbstractNode::ApplyTransformation(glm::mat4& model)
{
    glm::mat4 transformation = glm::mat4(1);
    transformation = glm::scale(transformation, scale_);
    transformation = glm::translate(transformation, position_);
    transformation = glm::rotate(transformation, rotation_[0], glm::vec3(1, 0, 0));
    transformation = glm::rotate(transformation, rotation_[2], glm::vec3(0, 0, 1));
    transformation = glm::rotate(transformation, rotation_[1], glm::vec3(0, 1, 0));
    model = transformation * model;
}

void AbstractNode::ApplyReflectionTransformation(glm::mat4& model)
{
    glm::mat4 transformation = glm::mat4(1);
    glm::vec3 tempPosition = position_;
    tempPosition.y = -tempPosition.y;
    transformation = glm::scale(transformation, scale_);
    transformation = glm::translate(transformation, tempPosition);
    transformation = glm::rotate(transformation, rotation_[0], glm::vec3(1, 0, 0));
    transformation = glm::rotate(transformation, rotation_[2], glm::vec3(0, 0, 1));
    transformation = glm::rotate(transformation, rotation_[1], glm::vec3(0, 1, 0));
    model = transformation * model;
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