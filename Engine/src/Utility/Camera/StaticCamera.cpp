#include <GL\glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "StaticCamera.h"
#include "Constant.h"
#include "Input/Input.h"

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif
#define DEG2RAD(a) (a / 180.0) * (M_PI)

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

void StaticCamera::Update(double deltaT)
{
    const double rotationSpeed = 120;
    const double mouvementSpeed = 10;
    if (Input::GetKeyState(Keys::Up) != Released)
    {
        phi_ -= DEG2RAD(rotationSpeed) * deltaT;
    }
    if (Input::GetKeyState(Keys::Down) != Released)
    {
        phi_ += DEG2RAD(rotationSpeed) * deltaT;
    }
    if (Input::GetKeyState(Keys::Left) != Released)
    {
        theta_ += DEG2RAD(rotationSpeed) * deltaT;
    }
    if (Input::GetKeyState(Keys::Right) != Released)
    {
        theta_ -= DEG2RAD(rotationSpeed) * deltaT;
    }
    if (Input::GetKeyState(Keys::Equal) != Released)
    {
        rho_ -= mouvementSpeed * deltaT;
    }
    if (Input::GetKeyState(Keys::Minus) != Released)
    {
        rho_ += mouvementSpeed * deltaT;
    }
}
void StaticCamera::Apply(glm::mat4& view, glm::mat4& projection)
{
    ApplyPerspective(projection);
    double x = rho_ * cos(theta_) * sin(phi_);
    double y = rho_ * cos(phi_); 
    double z = rho_ * sin(theta_) * sin(phi_);
    view = glm::lookAt(glm::vec3(x,y,z), lookAt_, up_);
}