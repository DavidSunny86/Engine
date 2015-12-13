#include "Environment.h"
#include "Light.h"
#include "Terrain/Terrain.h"

Environment::Environment()
{
    terrain_ = new Terrain();
}


Environment::~Environment()
{
}

void Environment::Render(const glm::mat4& view, const glm::mat4& projection)
{
    terrain_->Render(view, projection, lights_[0]);
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
