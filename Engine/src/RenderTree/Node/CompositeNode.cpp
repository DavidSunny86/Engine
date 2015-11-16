#include "CompositeNode.h"

CompositeNode::CompositeNode(AbstractNode* parent) : AbstractNode(parent)
{
}


CompositeNode::~CompositeNode()
{
    for (auto child : children_)
        delete child;
}

int CompositeNode::Depth() const
{
    int maxDepth = 0;
    for (auto child : children_)
    {
        maxDepth = child->Depth() > maxDepth ? child->Depth() : maxDepth;
    }
    return maxDepth + 1;
}

int CompositeNode::NumberOfChildren() const
{
    int numberOfChildren = children_.size();
    for (auto child : children_)
    {
        numberOfChildren += child->NumberOfChildren();
    }
    return numberOfChildren;
}

void CompositeNode::AddChild(AbstractNode * child)
{
    child->Parent(this);
    children_.push_back(child);
}

void CompositeNode::RemoveChild(AbstractNode * child)
{
    children_.remove(child);
}

void CompositeNode::Empty()
{
    children_.empty();
}
