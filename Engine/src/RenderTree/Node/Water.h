#pragma once
#include "RenderTree\Node\AbstractNode.h"
#include "Manager\GLSLProgramManager.h"
class Water :
    public AbstractNode
{
public:
    Water(AbstractNode* parent);
    virtual ~Water();

    virtual void Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane = glm::vec4(0));
    virtual void RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane, glm::mat4 shadowModel);
    virtual void RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection);
    virtual void Update(double deltaT);

    void ApplyReflectionTransformation(glm::mat4& modelReflection);

    void RenderModel(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, Environment* e, const glm::vec4& clipPlane = glm::vec4(0));
    void RenderModelFirstPass(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, const glm::vec4& clipPlane = glm::vec4(0));
    
    void LoadModel();
private:
    bool renderingWater_;
    void CreateBuffers();
    GLuint reflectionFbo_;
    GLuint reflectionTexture_;
    GLuint reflectionDepthBuffer_;
    
    GLuint refractionFbo_;
    GLuint refractionTexture_;
    GLuint refractionDepthBuffer_;

    GLuint textureWidth_;
    GLuint textureHeight_;

    GLSLProgram* waterProgram_;
    GLSLProgram* firstPassProgram_;
    Material* material_;
    GLuint vao_;
    GLuint vbo_[4];
    unsigned int numberOfElement_;
};

