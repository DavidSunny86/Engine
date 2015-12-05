#pragma once
#include <string.h>
class AbstractNode;
class NodeFactory
{
public:
    static AbstractNode* CreateNode(std::string& nodeName, AbstractNode* parent);
};

