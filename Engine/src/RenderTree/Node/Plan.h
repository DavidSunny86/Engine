#pragma once
#include "RenderTree\Node\AbstractNode.h"
class Plan :
    public AbstractNode
{
public:
    Plan(AbstractNode* parent);
    ~Plan();

    virtual void Update(double deltaT) override;
protected:
};

