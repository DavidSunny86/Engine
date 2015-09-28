#include "Scene.h"
#include "Utility/Camera/StaticCamera.h"
#include "Loader\EnvironmentLoader.h"

Scene::Scene()
{
    renderTree_ = new RenderTree(0);
    environment_ = EnvironmentLoader::CreateEnvironnement(".//Scenes//MainEnvironment.xml");
    camera_ = new StaticCamera(glm::vec3(10.f,10.f,30.f),glm::vec3(0.f,10.f,0.f),glm::vec3(0.f,1.f,0.f),0.1,100,45,16.f/9.f);
}


Scene::~Scene()
{
}

void Scene::Render()
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view;
    glm::mat4 projection;
    for (auto light : environment_->lights_)
    {
        light->Apply(view, projection);
        light->UseFrameBuffer();
        renderTree_->RenderShadowMap(model, view, projection);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1920, 1080);
    camera_->Apply(view, projection);
    environment_->TransformLight(model, view);
    renderTree_->RenderFirstPass(model, view, projection);
    renderTree_->Render(model, view, projection,environment_);
}

void Scene::Update(double deltaT)
{
    renderTree_->Update(deltaT);
}

void Scene::SetRenderTree(RenderTree* tree)
{
    renderTree_ = tree;
}

void Scene::SetEnvironment(Environment* environment)
{
    environment_ = environment;
}
