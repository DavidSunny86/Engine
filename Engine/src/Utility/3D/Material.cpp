#include <glm/gtc/type_ptr.hpp>
#include "Material.h"

Material::Material(aiMaterial* mtl)
{
    LoadMaterial(mtl);
    LoadTextures(mtl);
}

Material::~Material()
{

}

void Material::ApplyMaterial(GLSLProgram* program)
{
    glUniform4fv(program->GetUniformLocation("materialAmbient"), 1, glm::value_ptr(ambient_));
    glUniform4fv(program->GetUniformLocation("materialDiffuse"), 1, glm::value_ptr(diffuse_));
    glUniform4fv(program->GetUniformLocation("materialSpecular"), 1, glm::value_ptr(specular_));
    glUniform4fv(program->GetUniformLocation("materialEmission"), 1, glm::value_ptr(emissive_));
    glUniform1f(program->GetUniformLocation("shininess"), shininess_);
    ApplyTextures(program);
}

void Material::ApplyTextures(GLSLProgram* program)
{
    auto it = textures_.find(Diffuse);
    if (it != textures_.end())
    {
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, it->second->ID());
        glUniform1i(program->GetUniformLocation("diffuseTexture"),7);
    }
    it = textures_.find(Specular);
    if(it != textures_.end())
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, it->second->ID());
        glUniform1i(program->GetUniformLocation("specularTexture"), 1);
    }
    it = textures_.find(Ambient);
    if (it != textures_.end())
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, it->second->ID());
        glUniform1i(program->GetUniformLocation("ambientTexture"), 2);
    }
    it = textures_.find(Emissive);
    if (it != textures_.end())
    {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, it->second->ID());
        glUniform1i(program->GetUniformLocation("EmissiveTexture"), 3);
    }
    it = textures_.find(Height);
    if (it != textures_.end())
    {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, it->second->ID());
        glUniform1i(program->GetUniformLocation("HeightTexture"), 4);
    }
    it = textures_.find(Normals);
    if (it != textures_.end())
    {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, it->second->ID());
        glUniform1i(program->GetUniformLocation("NormalsTexture"), 5);
    }
    it = textures_.find(Shininess);
    if (it != textures_.end())
    {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, it->second->ID());
        glUniform1i(program->GetUniformLocation("ShininessTexture"), 6);
    }
}

void Material::LoadMaterial(aiMaterial* mtl)
{
    aiColor4D temp;
    aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &temp);
    TransferAIColorToVec4(temp, ambient_);
    aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &temp);
    TransferAIColorToVec4(temp, diffuse_);
    aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &temp);
    TransferAIColorToVec4(temp, specular_);
    aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &temp);
    TransferAIColorToVec4(temp, emissive_);
    unsigned int max = 1;
    aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess_, &max);


}


void Material::LoadTextures(aiMaterial* mtl)
{
    LoadTexturesType(mtl,aiTextureType_DIFFUSE);
    LoadTexturesType(mtl, aiTextureType_SPECULAR);
    LoadTexturesType(mtl, aiTextureType_AMBIENT);
    LoadTexturesType(mtl, aiTextureType_EMISSIVE);
    LoadTexturesType(mtl, aiTextureType_HEIGHT);
    LoadTexturesType(mtl, aiTextureType_NORMALS);
    LoadTexturesType(mtl, aiTextureType_SHININESS);
    LoadTexturesType(mtl, aiTextureType_OPACITY);
    LoadTexturesType(mtl, aiTextureType_DISPLACEMENT);
    LoadTexturesType(mtl, aiTextureType_LIGHTMAP);
    LoadTexturesType(mtl, aiTextureType_REFLECTION);
    LoadTexturesType(mtl, aiTextureType_UNKNOWN);
    LoadTexturesType(mtl, aiTextureType_NONE);
}

void Material::LoadTexturesType(aiMaterial* mtl, aiTextureType type)
{
    for (unsigned int i = 0; i < mtl->GetTextureCount(type); ++i)
    {
        aiString path;
        if (mtl->GetTexture(type, i, &path) == AI_SUCCESS)
        {
            std::string fullPath = std::string(".\\Textures\\");
            for (unsigned int j = 0; j < path.length; ++j)
                fullPath += path.data[j];
            textures_.insert(std::pair<TextureType,Texture*>(aiTextureTypeToTextureType(type),new Texture(fullPath,aiTextureTypeToTextureType(type))));
        }
    }
}

void Material::TransferAIColorToVec4(const aiColor4D& sender, glm::vec4& recipient)
{
    recipient.r = sender.r;
    recipient.g = sender.g;
    recipient.b = sender.b;
    recipient.a = sender.a;
}

TextureType Material::aiTextureTypeToTextureType(aiTextureType type)
{
    switch (type)
    {
    case aiTextureType_DIFFUSE:
        return Diffuse;
    case aiTextureType_SPECULAR:
        return Specular;
    case aiTextureType_AMBIENT:
        return Ambient;
    case aiTextureType_EMISSIVE:
        return Emissive;
    case aiTextureType_HEIGHT:
        return Height;
    case aiTextureType_NORMALS:
        return Normals;
    case aiTextureType_SHININESS:
        return Shininess;
    case aiTextureType_OPACITY:
        return Opacity;
    case aiTextureType_DISPLACEMENT:
        return Displacement;
    case aiTextureType_LIGHTMAP:
        return LightMap;
    case aiTextureType_REFLECTION:
        return Reflection;
    case aiTextureType_UNKNOWN:
        return Unkown;
    default:
        return None;
    }
}
