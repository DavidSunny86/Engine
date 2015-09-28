#include "Engine.h"
#include "GLFW\glfw3.h"
#include "Utility\Camera\StaticCamera.h"

Engine::Engine()
{
    window_ = new Window("Engine", 1920, 1080, false, false);
    scene_ = new Scene();
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

void Engine::ApplyOpenGLOptions()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_ONE, GL_ONE);

    glDepthFunc(GL_LEQUAL);
}
