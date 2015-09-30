#pragma once
#include "RenderTree/Node/RenderTree.h"
#include <string>
class RenderTreeLoader
{
public:
    static RenderTree* CreateRenderTree(std::string& pathToFile);
};

