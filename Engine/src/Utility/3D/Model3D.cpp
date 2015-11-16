#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Model3D.h"
#include "RenderTree\Environment\Environment.h"
#include "Manager\GLSLProgramManager.h"
Model3D::Model3D(std::string pathToFile)
{
    pathToFile_ = pathToFile;
    isLoaded_ = false;
}

Model3D::~Model3D()
{
    DeleteMeshes();
} 

void Model3D::Render(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, Environment* e)
{
    glUseProgram(program_->ID());
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(mv)));
    glUniformMatrix4fv(program_->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(program_->GetUniformLocation("MV"), 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix3fv(program_->GetUniformLocation("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    for (auto light : e->lights_)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light->shadowMap_);
        glUniform1i(program_->GetUniformLocation("shadowMap"), 0);
        glm::mat4 depthMVP = light->projection_ * light->view_ * m;
        depthMVP = light->dephtBiasMVP_ * depthMVP;
        glUniformMatrix4fv(program_->GetUniformLocation("depthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
        glUniform4fv(program_->GetUniformLocation("lightAmbientColor"), 1, glm::value_ptr(light->ambientColor_));
        glUniform4fv(program_->GetUniformLocation("lightDiffuseColor"), 1, glm::value_ptr(light->diffuseColor_));
        glUniform4fv(program_->GetUniformLocation("lightSpecularColor"), 1, glm::value_ptr(light->specularColor_));
        glUniform4fv(program_->GetUniformLocation("lightPosition"), 1, glm::value_ptr(light->position_));
        glUniform1f(program_->GetUniformLocation("lightIntensity"), light->power_);
        for (auto mesh : meshes_)
        {
            mesh->Render(program_);
        }
    }
    glDisable(GL_BLEND);
}

void Model3D::RenderFirstPass(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p)
{
    GLSLProgram* firstPassProgram = GLSLProgramManager::Instance()->GetProgram("FirstPass");
    glUseProgram(firstPassProgram->ID());
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glUniformMatrix4fv(firstPassProgram->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glDisable(GL_BLEND);
    for (auto mesh : meshes_)
    {
        mesh->RenderNoMaterial();
    }
}

void Model3D::RenderShadowMap(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p)
{
    GLSLProgram* shadowMapProgram = GLSLProgramManager::Instance()->GetProgram("ShadowMap");
    glUseProgram(shadowMapProgram->ID());
    glm::mat4 depthMVP = p * v * m;
    glUniformMatrix4fv(shadowMapProgram->GetUniformLocation("depthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
    for (auto mesh : meshes_)
    {
        mesh->RenderNoMaterial();
    }
}

void Model3D::RenderReflection(glm::mat4 m, const glm::mat4& v, const glm::mat4& p, Environment* e, const glm::vec4& clipPlane)
{
    RenderFirstPass(m, v, p);
    glUseProgram(program_->ID());
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(mv)));
    glUniformMatrix4fv(program_->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(program_->GetUniformLocation("MV"), 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix3fv(program_->GetUniformLocation("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    for (auto light : e->lights_)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light->shadowMap_);
        glUniform1i(program_->GetUniformLocation("shadowMap"), 0);
        glm::mat4 depthMVP = light->projection_ * light->view_ * m;
        depthMVP = light->dephtBiasMVP_ * depthMVP;
        depthMVP = glm::scale(depthMVP, glm::vec3(1, -1, 1));
        glUniformMatrix4fv(program_->GetUniformLocation("depthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
        glUniform4fv(program_->GetUniformLocation("lightAmbientColor"), 1, glm::value_ptr(light->ambientColor_));
        glUniform4fv(program_->GetUniformLocation("lightDiffuseColor"), 1, glm::value_ptr(light->diffuseColor_));
        glUniform4fv(program_->GetUniformLocation("lightSpecularColor"), 1, glm::value_ptr(light->specularColor_));
        glm::vec4 lightPosition = light->position_;
        lightPosition.y = -lightPosition.y;
        glUniform4fv(program_->GetUniformLocation("lightPosition"), 1, glm::value_ptr(lightPosition));
        glUniform1f(program_->GetUniformLocation("lightIntensity"), light->power_);
        glUniform4fv(program_->GetUniformLocation("clipPlane"), 1, glm::value_ptr(clipPlane));
        for (auto mesh : meshes_)
        {
            mesh->Render(program_);
        }
    }
    glDisable(GL_BLEND);
}

void Model3D::Program(GLSLProgram* program)
{
    program_ = program;
}

void Model3D::LoadModel()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(pathToFile_.c_str(), 
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if (!scene)
    {
        std::cerr << "Error loading model: " << pathToFile_ << std::endl;
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        auto mtl = scene->mMaterials[i];
        materials_.push_back(new Material(mtl));
    }
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        auto mesh = scene->mMeshes[i];
        meshes_.push_back(new Mesh(mesh));
        meshes_[i]->SetMaterial(materials_[mesh->mMaterialIndex]);
    }
    isLoaded_ = true;
}

bool Model3D::IsLoaded()
{
    return isLoaded_;
}

void Model3D::DeleteMeshes()
{
    for (auto mesh : meshes_)
        delete mesh;
    meshes_.clear();
}


