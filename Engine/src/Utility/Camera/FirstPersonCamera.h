#pragma once
#include "Utility\Camera\Camera.h"

class FirstPersonCamera :
    public Camera
{
public:
    FirstPersonCamera(float zNear, float zFar, float fov, float aspectRatio, AbstractNode * target);
    virtual ~FirstPersonCamera();

    virtual void Apply(glm::mat4& view, glm::mat4& projection);
protected:
};

