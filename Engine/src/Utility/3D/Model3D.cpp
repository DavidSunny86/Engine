#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Model3D.h"
#include "Mesh.h"
#include "Material.h"
#include "Manager\GLSLProgramManager.h"
#include "OpenGL\GLSLProgram.h"
#include "RenderTree\Environment\Light.h"
#include "RenderTree\Environment\Environment.h"
#include "OpenGL\GLSLProgram.h"


Model3D::Model3D(std::string pathToFile)
{
    pathToFile_ = pathToFile;
    isLoaded_ = false;
    firstPassProgram_ = GLSLProgramManager::Instance()->GetProgram("FirstPass");
    firstPassMVPLocation_ = firstPassProgram_->GetUniformLocation("MVP");
    firstPassMLocation_ = firstPassProgram_->GetUniformLocation("M");
    firstPassClipPlaneLocation_ = firstPassProgram_->GetUniformLocation("clipPlane");
    
    shadowMapProgram_ = GLSLProgramManager::Instance()->GetProgram("ShadowMap");
    shadowMapDepthMVPLocation_ = shadowMapProgram_->GetUniformLocation("depthMVP");
}

Model3D::~Model3D()
{
    DeleteMeshes();
} 

void Model3D::Render(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, Environment* e, const glm::vec4& clipPlane)
{
    glUseProgram(program_->ID());
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(mv)));
    glUniformMatrix4fv(programMVPLocation_, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(programMVLocation_, 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(programMLocation_, 1, GL_FALSE, glm::value_ptr(m));
    glUniformMatrix3fv(programNormalMatrixLocation_, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    for (auto light : e->lights_)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light->shadowMap_);
        glUniform1i(programShadowMapLocation_, 0);
        glm::mat4 depthMVP = light->projection_ * light->view_ * m;
        depthMVP = light->dephtBiasMVP_ * depthMVP;
        glUniformMatrix4fv(programDepthMVPLocation_, 1, GL_FALSE, glm::value_ptr(depthMVP));
        glUniform4fv(programLightAmbientColorLocation_, 1, glm::value_ptr(light->ambientColor_));
        glUniform4fv(programLightDiffuseColorLocation_, 1, glm::value_ptr(light->diffuseColor_));
        glUniform4fv(programLightSpecularColorLocation_, 1, glm::value_ptr(light->specularColor_));
        glUniform4fv(programLightPositionLocation_, 1, glm::value_ptr(light->transformedPosition_));
        glUniform1f(programLightIntensityLocation_, light->power_);
        for (auto mesh : meshes_)
        {
            mesh->Render(program_);
        }
    }
    glDisable(GL_BLEND);
}

void Model3D::RenderFirstPass(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, const glm::vec4& clipPlane)
{
    glUseProgram(firstPassProgram_->ID());
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glUniformMatrix4fv(firstPassMVPLocation_, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(firstPassMLocation_, 1, GL_FALSE, glm::value_ptr(m));
    glUniform4fv(firstPassClipPlaneLocation_, 1, glm::value_ptr(clipPlane));
    glDisable(GL_BLEND);
    for (auto mesh : meshes_)
    {
        mesh->RenderNoMaterial();
    }
}

void Model3D::RenderShadowMap(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p)
{
    glUseProgram(shadowMapProgram_->ID());
    glm::mat4 depthMVP = p * v * m;
    glUniformMatrix4fv(shadowMapDepthMVPLocation_, 1, GL_FALSE, glm::value_ptr(depthMVP));
    for (auto mesh : meshes_)
    {
        mesh->RenderNoMaterial();
    }
}

void Model3D::RenderReflection(glm::mat4 m, const glm::mat4& v, const glm::mat4& p, Environment* e, const glm::vec4& clipPlane, glm::mat4 shadowModel)
{
    RenderFirstPass(m, v, p, clipPlane);
    glUseProgram(program_->ID());
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(mv)));
    glUniformMatrix4fv(programMVPLocation_, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(programMVLocation_, 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(programMLocation_, 1, GL_FALSE, glm::value_ptr(m));
    glUniformMatrix3fv(programNormalMatrixLocation_, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    for (auto light : e->lights_)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light->shadowMap_);
        glUniform1i(programShadowMapLocation_, 0);
        shadowModel = glm::scale(shadowModel, glm::vec3(-1, 1, 1));
        glm::mat4 depthMVP = light->projection_ * light->view_ * shadowModel;
        depthMVP = light->dephtBiasMVP_ * depthMVP;
        glUniformMatrix4fv(programDepthMVPLocation_, 1, GL_FALSE, glm::value_ptr(depthMVP));
        glUniform4fv(programLightAmbientColorLocation_, 1, glm::value_ptr(light->ambientColor_));
        glUniform4fv(programLightDiffuseColorLocation_, 1, glm::value_ptr(light->diffuseColor_));
        glUniform4fv(programLightSpecularColorLocation_, 1, glm::value_ptr(light->specularColor_));
        glUniform4fv(programLightPositionLocation_, 1, glm::value_ptr(light->transformedPosition_));
        glUniform1f(programLightIntensityLocation_, light->power_);
        glUniform4fv(programClipPlaneLocation_, 1, glm::value_ptr(clipPlane));
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
    programMVPLocation_ = program_->GetUniformLocation("MVP");
    programMVLocation_ = program_->GetUniformLocation("MV");
    programMLocation_ = program->GetUniformLocation("M");
    programNormalMatrixLocation_ = program_->GetUniformLocation("normalMatrix");
    programClipPlaneLocation_ = program_->GetUniformLocation("clipPlane");

    programShadowMapLocation_ = program_->GetUniformLocation("shadowMap");
    programDepthMVPLocation_ = program_->GetUniformLocation("depthMVP");
    programLightAmbientColorLocation_ = program_->GetUniformLocation("lightAmbientColor");
    programLightDiffuseColorLocation_ = program_->GetUniformLocation("lightDiffuseColor");
    programLightSpecularColorLocation_ = program_->GetUniformLocation("lightSpecularColor");
    programLightPositionLocation_ = program_->GetUniformLocation("lightPosition");
    programLightIntensityLocation_ = program_->GetUniformLocation("lightIntensity");
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


