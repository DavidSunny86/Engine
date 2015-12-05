#pragma once
#include <string>
class Environment;

class EnvironmentLoader
{
public:
    static Environment* CreateEnvironment(std::string& pathToFile);
};

