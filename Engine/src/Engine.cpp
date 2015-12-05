#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Engine.h"
#include "Utility\Window.h"
#include "RenderTree\Scene.h"
#include "Utility\Camera\StaticCamera.h"
#include "Constant.h"

Engine* Engine::instance_ = nullptr;

Engine::Engine()
{
    window_ = new Window("Engine", Constant::ViewportWidth, Constant::ViewPortHeight, false, true);
    scene_ = new Scene();
	Window::scene_ = scene_;
}


Engine::~Engine()
{
    delete window_;
    delete scene_;
}

void Engine::Loop()
{
    double time = glfwGetTime();
    while (!window_->ShouldClose())
    {
        double deltaTime = glfwGetTime() - time;
        time += deltaTime;
        
        window_->PollEvent();
        scene_->Update(deltaTime);
        ApplyOpenGLOptions();
        scene_->Render();
        window_->SwapBuffer();
    }
}

Engine* Engine::Instance()
{
    if (instance_ == nullptr)
        instance_ = new Engine();
    return instance_;
}

Scene* Engine::GetScene()
{
    return scene_;
}

Window* Engine::GetWindow()
{
    return window_;
}

void Engine::ApplyOpenGLOptions()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_ONE, GL_ONE);
    glDepthFunc(GL_LEQUAL);
}
