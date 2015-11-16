#pragma once
#include "RenderTree\Node\AbstractNode.h"
#include "Utility\3D\Model3D.h"
class Plan :
    public AbstractNode
{
public:
    Plan(AbstractNode* parent);
    ~Plan();

    virtual void Update(double deltaT);
protected:
};

