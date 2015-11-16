#include "RenderTreeLoader.h"
#include <TiniXML2/tinyxml2.h>
#include "Loader/NodeFactory.h"

RenderTree* RenderTreeLoader::CreateRenderTree(std::string& pathToFile)
{
    RenderTree* renderTree = new RenderTree(0);
    tinyxml2::XMLDocument document;
    document.LoadFile(pathToFile.c_str());
    tinyxml2::XMLNode* mainNode = document.FirstChild();
    tinyxml2::XMLNode* child = mainNode->FirstChildElement();
    while (child)
    {
        std::string nodeType = child->Value();
        AbstractNode* newNode = NodeFactory::CreateNode(nodeType, renderTree);


        float x, y, z;
        tinyxml2::XMLElement* element = child->FirstChildElement("Position");
        x = element->FloatAttribute("x");
        y = element->FloatAttribute("y");
        z = element->FloatAttribute("z");
        newNode->Position(glm::vec3(x, y, z));
        element = child->FirstChildElement("Rotation");
        x = element->FloatAttribute("x");
        y = element->FloatAttribute("y");
        z = element->FloatAttribute("z");
        newNode->Rotation(glm::vec3(x, y, z));
        element = child->FirstChildElement("Scale");
        x = element->FloatAttribute("x");
        y = element->FloatAttribute("y");
        z = element->FloatAttribute("z");
        newNode->Scale(glm::vec3(x, y, z));
        
        std::string model;
        element = child->FirstChildElement("Model");
        model = element->Attribute("Name");
        newNode->Model(model);
        renderTree->AddChild(newNode);
        child = child->NextSibling();
    }
    return renderTree;
}
