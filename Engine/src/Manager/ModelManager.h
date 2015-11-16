#pragma once
#include <map>
#include <string>

#include "Utility/3D/Model3D.h"


class ModelManager
{
public:
    ~ModelManager();

    static ModelManager* Instance();
    Model3D* GetModel(const std::string& name);
protected:
    ModelManager();
    void LoadModels(std::string& path);
    
    static ModelManager* instance_;
    std::map<std::string, Model3D*> models_;
};

