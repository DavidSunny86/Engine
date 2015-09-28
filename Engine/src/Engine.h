#pragma once
#include "Utility\Window.h"
#include "RenderTree\Scene.h"

class Engine
{
public:
    Engine();
    ~Engine();
    void Loop();
protected:
    void ApplyOpenGLOptions();

    Window* window_;
    Scene* scene_;
};

