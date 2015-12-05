#pragma once

class Scene;
class Window;

class Engine
{
public:

    ~Engine();
    void Loop();
    static Engine* Instance();
    Scene* GetScene();
    Window* GetWindow();
protected:
    Engine();
    void ApplyOpenGLOptions();
    static Engine* instance_;
    Window* window_;
    Scene* scene_;
};

