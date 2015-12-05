#pragma once
#include <glm/glm.hpp>
#include <assimp/material.h>
#include <map>

#include "Texture.h"

class GLSLProgram;
class Texture;

class Material
{
public:
    Material(aiMaterial* mtl);
    ~Material();

    void ApplyMaterial(GLSLProgram* program);
protected:
    void ApplyTextures(GLSLProgram* program);

    void LoadMaterial(aiMaterial* mtl);
    void LoadTextures(aiMaterial* mtl);
    void LoadTexturesType(aiMaterial* mtl, aiTextureType type);
    void TransferAIColorToVec4(const aiColor4D& sender, glm::vec4& recipient);
    
    TextureType aiTextureTypeToTextureType(aiTextureType type);
    std::map<TextureType, Texture*> textures_;
    glm::vec4 ambient_;
    glm::vec4 diffuse_;
    glm::vec4 specular_;
    glm::vec4 emissive_;
    float shininess_;
};