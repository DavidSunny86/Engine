#include "StaticCamera.h"
#include "GL\glew.h"
#include <glm/gtc/matrix_transform.hpp>
StaticCamera::StaticCamera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up, float zNear, float zFar, float fov, float aspectRatio)
    : Camera(position, lookAt, up, zNear, zFar, fov, aspectRatio, 0)
{
}


StaticCamera::~StaticCamera()
{
}

void StaticCamera::Apply(glm::mat4& view, glm::mat4& projection)
{
    //position_[0] -= 0.1;
    ApplyPerspective(projection);
    view = glm::lookAt(position_, lookAt_, up_);
}
