#include <GLFW/glfw3.h>

#include "KeyboardKeyManager.h"
#include "Input/Handler/IKeyboardKeyHandler.h"

KeyboardKeyManager* KeyboardKeyManager::instance_ = nullptr;

KeyboardKeyManager::KeyboardKeyManager()
{
}


KeyboardKeyManager::~KeyboardKeyManager()
{
}

KeyboardKeyManager* KeyboardKeyManager::Instance()
{
    if (instance_ == nullptr)
        instance_ = new KeyboardKeyManager();
    return instance_;
}

void KeyboardKeyManager::KeyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifier)
{
    Instance()->DispatchEvent(key, action, modifier);
}

void KeyboardKeyManager::Subscribe(IKeyboardKeyHandler* subscriber)
{
    subscribers_.push_back(subscriber);
}

void KeyboardKeyManager::Unsubscribe(IKeyboardKeyHandler* subscriber)
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

void KeyboardKeyManager::DispatchEvent(int key, int action, int modifier)
{
    for (auto subscriber : subscribers_)
        subscriber->HandleKeyboardKey(key, action, modifier);
}
