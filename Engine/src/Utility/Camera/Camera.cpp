#include "Camera.h"
#include "GL\glew.h"
Camera::Camera(const glm::vec3& position,const glm::vec3& lookAt, const glm::vec3& up, float zNear, float zFar, float fov, float aspectRatio, AbstractNode * target)
{
    position_ = position;
    lookAt_ = lookAt;
    up_ = up;
    zNear_ = zNear;
    zFar_ = zFar;
    fov_ = fov;
    aspectRatio_ = aspectRatio;
    target_ = target;
}

Camera::~Camera()
{
}

const glm::vec3 & Camera::Position() const
{
    return position_;
}

const glm::vec3 & Camera::LookAt() const
{
    return lookAt_;
}

void Camera::Up(const glm::vec3& up)
{
    up_ = up;
}

const glm::vec3& Camera::Up() const
{
    return up_;
}

AbstractNode * Camera::Target() const
{
    return target_;
}

void Camera::Position(const glm::vec3 & position)
{
    position_ = position;
}

void Camera::LookAt(const glm::vec3 & lookAt)
{
    lookAt_= lookAt;
}

void Camera::Target(AbstractNode * target)
{
    target_ = target;
}

void Camera::ApplyPerspective(glm::mat4& projection) const
{
    projection = glm::perspective(fov_, aspectRatio_, zNear_, zFar_);
}
