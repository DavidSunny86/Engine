#include <TiniXML2/tinyxml2.h>

#include "EnvironmentLoader.h"
#include "RenderTree/Environment/Environment.h"
#include "RenderTree/Environment/Light.h"

Environment* EnvironmentLoader::CreateEnvironment(std::string& pathToFile)
{
    Environment* env = new Environment();
    tinyxml2::XMLDocument document;
    document.LoadFile(pathToFile.c_str());
    tinyxml2::XMLNode* mainNode = document.FirstChild();
    tinyxml2::XMLNode* child = mainNode->FirstChildElement("Light");
    while (child)
    {
        Light* l = new Light();
        tinyxml2::XMLElement* element = child->FirstChildElement("Position");
        l->position_[0] = element->FloatAttribute("x");
        l->position_[1] = element->FloatAttribute("y");
        l->position_[2] = element->FloatAttribute("z");
        l->position_[3] = element->FloatAttribute("w");
        element = child->FirstChildElement("Ambient");
        l->ambientColor_[0] = element->FloatAttribute("r");
        l->ambientColor_[1] = element->FloatAttribute("g");
        l->ambientColor_[2] = element->FloatAttribute("b");
        l->ambientColor_[3] = element->FloatAttribute("a");
        element = child->FirstChildElement("Diffuse");
        l->diffuseColor_[0] = element->FloatAttribute("r");
        l->diffuseColor_[1] = element->FloatAttribute("g");
        l->diffuseColor_[2] = element->FloatAttribute("b");
        l->diffuseColor_[3] = element->FloatAttribute("a");
        element = child->FirstChildElement("Specular");
        l->specularColor_[0] = element->FloatAttribute("r");
        l->specularColor_[1] = element->FloatAttribute("g");
        l->specularColor_[2] = element->FloatAttribute("b");
        l->specularColor_[3] = element->FloatAttribute("a");
        element = child->FirstChildElement("Intensity");
        l->power_ = element->FloatAttribute("i");
        env->AddLight(l);
        child = child->NextSibling();
    }
    return env;
}
