#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MousePositionManager.h"
#include "Input/Handler/IMousePositionHandler.h"

MousePositionManager* MousePositionManager::instance_ = nullptr;

void MousePositionManager::DispatchEvent(double xPos, double yPos)
{
    for (auto subscriber : subscribers_)
        subscriber->HandleMousePosition(xPos, yPos);
}

MousePositionManager::MousePositionManager()
{
}

MousePositionManager::~MousePositionManager()
{
}

MousePositionManager* MousePositionManager::Instance()
{
    if (instance_ == nullptr)
        instance_ = new MousePositionManager();
    return instance_;
}

void MousePositionManager::MousePositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    Instance()->DispatchEvent(xPos, yPos);
}

void MousePositionManager::Subscribe(IMousePositionHandler* subscriber)
{
    subscribers_.push_back(subscriber);
}

void MousePositionManager::Unsubscribe(IMousePositionHandler* subscriber)
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
