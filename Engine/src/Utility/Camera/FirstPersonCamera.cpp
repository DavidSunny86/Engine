#include <GL\glew.h>
#include <glm/common.hpp>
#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera( float zNear, float zFar, float fov, float aspectRatio, AbstractNode * target) :
    Camera(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0),zNear,zFar,fov,aspectRatio,target)
{
}


FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Apply(glm::mat4& view, glm::mat4& projection)
{

}
