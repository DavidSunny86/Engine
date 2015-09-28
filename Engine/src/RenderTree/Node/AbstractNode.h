#pragma once
#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "OpenGL\GLSLProgram.h"
#include "RenderTree\Environment\Environment.h"

class AbstractNode
{
public:
    AbstractNode(AbstractNode* parent);
    virtual ~AbstractNode();

    virtual void Update(double deltaT) = 0;
    virtual void Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement) = 0;
    virtual void RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection) = 0;
    virtual void RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection) = 0;
    void ApplyTransformation(glm::mat4& model);

    //Tree Functionality
    AbstractNode* Parent() const;
    virtual int Depth() const;
    virtual int NumberOfChildren() const;
    virtual void AddChild(AbstractNode* child);
    virtual void RemoveChild(AbstractNode* child);
    virtual void Empty();

    //Accessor
    glm::vec3& Position();
    glm::vec3& Rotation();
    glm::vec3& Scale();

    void Position(const glm::vec3& position);
    void Rotation(const glm::vec3& rotation);
    void Scale(const glm::vec3& scale);
protected:
    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;

    AbstractNode* parent_;
};

