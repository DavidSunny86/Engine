#pragma once
#include <vector>

class IKeyboardKeyHandler;
struct GLFWwindow;

class KeyboardKeyManager
{
public:
    KeyboardKeyManager();
    ~KeyboardKeyManager();
    static KeyboardKeyManager* Instance();
    static void KeyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifier);
    void Subscribe(IKeyboardKeyHandler* subscriber);
    void Unsubscribe(IKeyboardKeyHandler* subscriber);
protected:
    void DispatchEvent(int key, int action, int modifier);
    std::vector<IKeyboardKeyHandler*> subscribers_;

    static KeyboardKeyManager* instance_;
};

