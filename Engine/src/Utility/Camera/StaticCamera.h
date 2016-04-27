#pragma once
#include "Utility\Camera\Camera.h"
class StaticCamera :
    public Camera
{
public:
    StaticCamera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up, float zNear, float zFar, float fov, float aspectRatio);
    virtual ~StaticCamera();
    virtual void Update(double deltaT) override;
    virtual void Apply(glm::mat4& view, glm::mat4& projection) override;
protected:
    double theta_;
    double phi_;
    double rho_;
};

