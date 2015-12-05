#pragma once
#include <vector>
#include <string>
#include <glm/common.hpp>
#include <GL/glew.h>

class Mesh;
class Material;
class GLSLProgram;
class Environment;

class Model3D
{
public:
    Model3D(std::string pathToFile);
    ~Model3D();

    void Render(const glm::mat4& m,const glm::mat4& v,const glm::mat4& p, Environment* e, const glm::vec4& clipPlane = glm::vec4(0));
    void RenderFirstPass(const glm::mat4& m,const glm::mat4& v,const glm::mat4& p, const glm::vec4& clipPlane = glm::vec4(0));
    void RenderShadowMap(const glm::mat4& m,const glm::mat4& v,const glm::mat4& p);
    void RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane, glm::mat4 shadowModel);

    void Program(GLSLProgram* program);
    void LoadModel();
    bool IsLoaded();
protected:
    void DeleteMeshes();


    std::vector<Material*> materials_;
    std::vector<Mesh*> meshes_;
    std::string pathToFile_;
    GLSLProgram* program_;
    GLSLProgram* firstPassProgram_;
    GLSLProgram* shadowMapProgram_;

    GLint firstPassMVPLocation_;
    GLint firstPassMLocation_;
    GLint firstPassClipPlaneLocation_;

    GLint shadowMapDepthMVPLocation_;
    
    GLint programMVPLocation_;
    GLint programMVLocation_;
    GLint programMLocation_;
    GLint programNormalMatrixLocation_;
    GLint programClipPlaneLocation_;

    GLint programShadowMapLocation_;
    GLint programDepthMVPLocation_;
    GLint programLightAmbientColorLocation_;
    GLint programLightDiffuseColorLocation_;
    GLint programLightSpecularColorLocation_;
    GLint programLightPositionLocation_;
    GLint programLightIntensityLocation_;

    bool isLoaded_;
};

