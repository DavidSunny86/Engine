#pragma once
#include <vector>
#include <glm/common.hpp>

class Light;
class Terrain;

class Environment
{
public:
    Environment();
    ~Environment();
    void Render(const glm::mat4& view, const glm::mat4& projection);
    void TransformLight(const glm::mat4& model, const glm::mat4& view);

    void AddLight(Light* light);
    std::vector<Light*> lights_;
protected:
    Terrain* terrain_;
};

