#pragma once
#include "RenderTree\Node\AbstractNode.h"

class Human :
    public AbstractNode
{
public:
    Human(AbstractNode* parent);
    ~Human();
protected:
};

