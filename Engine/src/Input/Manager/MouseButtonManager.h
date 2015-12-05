#pragma once
#include <vector>

class IMouseButtonHandler;
struct GLFWwindow;

class MouseButtonManager
{
public:
    MouseButtonManager();
    ~MouseButtonManager();

    static MouseButtonManager* Instance();
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int modifier);
    void Subscribe(IMouseButtonHandler* subscriber);
    void Unsubscribe(IMouseButtonHandler* subscriber);
protected:
    void DispatchEvent(int button, int action, int modifier);
    std::vector<IMouseButtonHandler*> subscribers_;

    static MouseButtonManager* instance_;
};

