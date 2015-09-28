#pragma once
#include <list>

#include "RenderTree\Node\AbstractNode.h"

class CompositeNode :
    public AbstractNode
{
public:
    CompositeNode(AbstractNode* parent);
    virtual ~CompositeNode();

    virtual int Depth() const;
    virtual int NumberOfChildren() const;
    virtual void AddChild(AbstractNode* child);
    virtual void RemoveChild(AbstractNode* child);
    virtual void Empty();
protected:
    std::list<AbstractNode*> children_;
};

