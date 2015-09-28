#pragma once
#include "RenderTree/Environment/Environment.h"
class EnvironmentLoader
{
public:
    static Environment* CreateEnvironnement(std::string pathToFile);
};

