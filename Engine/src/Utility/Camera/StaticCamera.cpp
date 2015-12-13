#include <GL\glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "StaticCamera.h"
#include "Constant.h"

#ifndef M_PI
#define M_PI 3.1415926535897932385f
#endif
#define DEG2RAD(a) (a / 180.0f) * (M_PI);

StaticCamera::StaticCamera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up, float zNear, float zFar, float fov, float aspectRatio)
    : Camera(position, lookAt, up, zNear, zFar, fov, aspectRatio, 0)
    , theta_(1.57f)
    , phi_(1.57f / 2.0f)
    , rho_(4.f)
{
    
}


StaticCamera::~StaticCamera()
{
}

void StaticCamera::Apply(glm::mat4& view, glm::mat4& projection)
{
    ApplyPerspective(projection);
    double x = rho_ * cos(theta_) * sin(phi_);
    double y = rho_ * cos(phi_); 
    double z = rho_ * sin(theta_) * sin(phi_);
    view = glm::lookAt(glm::vec3(x,y,z), lookAt_, up_);
}

void StaticCamera::HandleKeyboardKey(int key, int action, int modifier)
{
    if (action == GLFW_REPEAT || action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_RIGHT:
            theta_ -= DEG2RAD(2.0f);
            break;
        case GLFW_KEY_LEFT:
            theta_ += DEG2RAD(2.0f);
            break;
        case GLFW_KEY_UP:
            phi_ -= DEG2RAD(2.0f);
            break;
        case GLFW_KEY_DOWN:
            phi_ += DEG2RAD(2.0f);
            break;
        case GLFW_KEY_EQUAL:
            rho_ -= 0.5;
            break;
        case GLFW_KEY_MINUS:
            rho_ += 0.5;
            break;
        }
    }
}
