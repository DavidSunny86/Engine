#pragma once
#include <vector>
#include <string>

#include "Mesh.h"
#include "OpenGL/GLSLProgram.h"
#include "RenderTree/Environment/Environment.h"

class Model3D
{
public:
    Model3D(std::string pathToFile);
    ~Model3D();

    void Render(const glm::mat4& m,const glm::mat4& v,const glm::mat4& p, Environment* e);
    void RenderFirstPass(const glm::mat4& m,const glm::mat4& v,const glm::mat4& p);
    void RenderShadowMap(const glm::mat4& m,const glm::mat4& v,const glm::mat4& p);
    void RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane);

    void Program(GLSLProgram* program);
    void LoadModel();
    bool IsLoaded();
protected:
    void DeleteMeshes();

    std::vector<Material*> materials_;
    std::vector<Mesh*> meshes_;
    std::string pathToFile_;
    GLSLProgram* program_;
    bool isLoaded_;
};

