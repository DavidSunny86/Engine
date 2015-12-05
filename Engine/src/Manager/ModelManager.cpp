#include <TiniXML2/tinyxml2.h>
#include <iostream>

#include "ModelManager.h"
#include "GLSLProgramManager.h"
#include "Utility/3D/Model3D.h"

#ifndef XMLCheckResult
#define XMLCheckResult(result) if (result != tinyxml2::XML_SUCCESS){ std::cerr << "Unable to open ModelManager file: " << path << std::endl; return;}
#endif

ModelManager* ModelManager::instance_ = NULL;

ModelManager::ModelManager()
{
    LoadModels(std::string(".\\Models\\Models.xml"));
}


void ModelManager::LoadModels(std::string& path)
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError result = document.LoadFile(path.c_str());
    XMLCheckResult(result);
    tinyxml2::XMLNode* root = document.FirstChild();
    tinyxml2::XMLElement* element = root->FirstChildElement();
    do
    {
        std::string name = element->Name();
        std::string pathToFile = element->Attribute("File");
        std::string program = element->Attribute("Program");
        Model3D* model = new Model3D(std::string(".\\Models\\"+ pathToFile));
        model->Program(GLSLProgramManager::Instance()->GetProgram(program));
        models_.insert(std::pair<std::string, Model3D*>(name, model));
    } while (element = element->NextSiblingElement());
}

ModelManager::~ModelManager()
{
}

ModelManager* ModelManager::Instance()
{
    if (instance_ == NULL)
        instance_ = new ModelManager();
    return instance_;
}

Model3D* ModelManager::GetModel(const std::string& name)
{
    auto model = models_.find(name);
    if (model != models_.end())
    {
        if (!(*model).second->IsLoaded())
            ((*model).second->LoadModel());
        return (*model).second;
    }

    else
        return NULL;
}
