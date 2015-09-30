#include <iostream>

#include "NodeFactory.h"
#include "RenderTree/Node/Human.h"
#include "RenderTree/Node/Plan.h"

AbstractNode* NodeFactory::CreateNode(std::string& nodeName, AbstractNode* parent)
{
    if (nodeName == "Human")
    {
        return new Human(parent);
    }
    else if (nodeName == "Plan")
    {
        return new Plan(parent);
    }
    throw new std::invalid_argument("Node type doesn't exist");
}
