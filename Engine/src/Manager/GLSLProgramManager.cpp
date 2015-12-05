#include <TiniXML2/tinyxml2.h>
#include <iostream>

#include "GLSLProgramManager.h"
#include "OpenGL/GLSLProgram.h"
#include "OpenGL/GLSLShader.h"

#ifndef XMLCheckResult
#define XMLCheckResult(result) if (result != tinyxml2::XML_SUCCESS){ std::cerr << "Unable to open ModelManager file: " << path << std::endl; return;}
#endif

GLSLProgramManager* GLSLProgramManager::instance_ = NULL;

void GLSLProgramManager::LoadPrograms(std::string& path)
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError result = document.LoadFile(path.c_str());
    XMLCheckResult(result);
    tinyxml2::XMLNode* root = document.FirstChild();
    tinyxml2::XMLElement* element = root->FirstChildElement();
    do
    {
        GLSLProgram* program = NULL;
        std::string name = element->Name();
        if (name.find("Compute") != std::string::npos)
        {
            std::string compute = element->Attribute("Compute");
            GLSLShader* shader = new GLSLShader(GLSLShaderType::Compute, ".\\Shaders\\" + compute);
            std::vector<GLSLShader*> shaders;
            shaders.push_back(shader);
            program = new GLSLProgram(shaders);
        }
        else
        {
            std::string vertex = element->Attribute("Vertex");
            std::string fragment = element->Attribute("Fragment");
            program = new GLSLProgram(std::string(".\\Shaders\\" + vertex), std::string(".\\Shaders\\" + fragment));
        }
        programs_.insert(std::pair<std::string, GLSLProgram*>(name, program));
    } while (element = element->NextSiblingElement());
}

GLSLProgramManager::GLSLProgramManager(std::string& path)
{
    LoadPrograms(path);
}


GLSLProgramManager::~GLSLProgramManager()
{
}

GLSLProgramManager* GLSLProgramManager::Instance()
{
    if (instance_ == NULL)
        instance_ = new GLSLProgramManager(std::string(".\\Shaders\\Programs.xml"));
    return instance_;
}

GLSLProgram* GLSLProgramManager::GetProgram(std::string name)
{
    auto program = programs_.find(name);
    if (program != programs_.end())
        return (*program).second;
    else
        return NULL;
}
