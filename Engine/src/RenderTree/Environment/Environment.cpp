#include "Environment.h"



Environment::Environment()
{
}


Environment::~Environment()
{
}

void Environment::TransformLight(const glm::mat4& model, const glm::mat4& view)
{
    glm::mat4 modelView = view * model;
    for (auto light : lights_)
    {
        light->TransformPosition(modelView);
    }
}

void Environment::AddLight(Light* light)
{
    lights_.push_back(light);
}
