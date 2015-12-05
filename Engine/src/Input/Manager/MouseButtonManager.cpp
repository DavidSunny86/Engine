#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MouseButtonManager.h"
#include "Input/Handler/IMouseButtonHandler.h"

MouseButtonManager* MouseButtonManager::instance_ = nullptr;

MouseButtonManager::MouseButtonManager()
{
}


MouseButtonManager::~MouseButtonManager()
{
}

MouseButtonManager* MouseButtonManager::Instance()
{
    if (instance_ == nullptr)
        instance_ = new MouseButtonManager();
    return instance_;
}

void MouseButtonManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int modifier)
{
    Instance()->DispatchEvent(button, action, modifier);
}

void MouseButtonManager::Subscribe(IMouseButtonHandler* subscriber)
{
    subscribers_.push_back(subscriber);
}

void MouseButtonManager::Unsubscribe(IMouseButtonHandler* subscriber)
{
    for (auto it = subscribers_.begin(); it != subscribers_.end(); ++it)
    {
        if ((*it) == subscriber)
        {
            subscribers_.erase(it);
            break;
        }
    }
}

void MouseButtonManager::DispatchEvent(int button, int action, int modifier)
{
    for (auto subscriber : subscribers_)
        subscriber->HandleMouseKey(button, action, modifier);
}
