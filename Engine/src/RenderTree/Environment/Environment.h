#pragma once
#include <vector>

#include "Light.h"
class Environment
{
public:
    Environment();
    ~Environment();

    void TransformLight(const glm::mat4& model, const glm::mat4& view);

    void AddLight(Light* light);
    std::vector<Light*> lights_;
};

