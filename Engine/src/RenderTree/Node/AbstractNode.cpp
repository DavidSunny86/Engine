#include "AbstractNode.h"
#include "GL\glew.h"


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

void AbstractNode::ApplyTransformation(glm::mat4& model)
{
    glm::mat4 transformation = glm::mat4(1);
    transformation = glm::translate(transformation, position_);
    transformation = glm::rotate(transformation, rotation_[0], glm::vec3(1, 0, 0));
    transformation = glm::rotate(transformation, rotation_[2], glm::vec3(0, 0, 1));
    transformation = glm::rotate(transformation, rotation_[1], glm::vec3(0, 1, 0));
    transformation = glm::scale(transformation, scale_);
    model = transformation * model;
}

AbstractNode * AbstractNode::Parent() const
{
    return parent_;
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