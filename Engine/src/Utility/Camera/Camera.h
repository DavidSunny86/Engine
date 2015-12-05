#pragma once
#include <glm/common.hpp>

class AbstractNode;

class Camera
{
public:
    Camera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up, float zNear, float zFar, float fov,float aspectRatio, AbstractNode* target = nullptr);
    virtual ~Camera();

    virtual void Apply(glm::mat4& view, glm::mat4& projection) = 0;

    const glm::vec3& Position() const;
    const glm::vec3& LookAt() const;
    const glm::vec3& Up() const;
    AbstractNode* Target() const;

    void Position(const glm::vec3& position);
    void LookAt(const glm::vec3& lookAt);
    void Up(const glm::vec3& up);
    void Target(AbstractNode* target);

protected:
    void ApplyPerspective(glm::mat4& projection) const;

    glm::vec3 position_;
    glm::vec3 lookAt_;
    glm::vec3 up_;
    float zNear_;
    float zFar_;
    float fov_;
    float aspectRatio_;
    AbstractNode* target_;
};

