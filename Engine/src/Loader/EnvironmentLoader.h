#pragma once
#include "RenderTree/Environment/Environment.h"
class EnvironmentLoader
{
public:
    static Environment* CreateEnvironment(std::string& pathToFile);
};

