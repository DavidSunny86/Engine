#pragma once
#include <string>
#include <glm\common.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL/glew.h>

class GLSLProgram;
class Environment;
class Model3D;

class AbstractNode
{
public:
    AbstractNode(AbstractNode* parent);
    virtual ~AbstractNode();

    virtual void Update(double deltaT);
    virtual void Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane = glm::vec4(0));
    virtual void RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection);
    virtual void RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection);
    virtual void RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane, glm::mat4 shadowModel);
    void ApplyTransformation(glm::mat4& model);

    //Tree Functionality
    AbstractNode* Parent() const;
    void Parent(AbstractNode* parent);
    virtual int Depth() const;
    virtual int NumberOfChildren() const;
    virtual void AddChild(AbstractNode* child);
    virtual void RemoveChild(AbstractNode* child);
    virtual void Empty();

    //Accessor
    glm::vec3& Position();
    glm::vec3& Rotation();
    glm::vec3& Scale();
    Model3D* Model();

    void Position(const glm::vec3& position);
    void Rotation(const glm::vec3& rotation);
    void Scale(const glm::vec3& scale);
    void Model(const std::string& name);
protected:
    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;

    AbstractNode* parent_;
    Model3D* model_;
};

