#pragma once
#include "Utility\Camera\Camera.h"
#include "Input\Handler\IKeyboardKeyHandler.h"
class StaticCamera :
    public Camera,
    public IKeyboardKeyHandler
{
public:
    StaticCamera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up, float zNear, float zFar, float fov, float aspectRatio);
    virtual ~StaticCamera();
    virtual void Apply(glm::mat4& view, glm::mat4& projection) override;
    virtual void HandleKeyboardKey(int key, int action, int modifier) override;
protected:
    float theta_;
    float phi_;
    float rho_;
};

