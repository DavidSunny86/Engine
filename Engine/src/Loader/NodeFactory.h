#pragma once
#include <string.h>
#include "RenderTree/Node/AbstractNode.h"
class NodeFactory
{
public:
    static AbstractNode* CreateNode(std::string& nodeName, AbstractNode* parent);
};

