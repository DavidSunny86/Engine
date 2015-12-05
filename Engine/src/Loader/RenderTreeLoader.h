#pragma once
#include <string>

class RenderTree;

class RenderTreeLoader
{
public:
    static RenderTree* CreateRenderTree(std::string& pathToFile);
};

