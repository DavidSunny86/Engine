#pragma once
#include <vector>

class IMousePositionHandler;
struct GLFWwindow;

class MousePositionManager
{
public:
    MousePositionManager();
    ~MousePositionManager();

    static MousePositionManager* Instance();
    static void MousePositionCallback(GLFWwindow* window, double xPos, double yPos);
    void Subscribe(IMousePositionHandler* subscriber);
    void Unsubscribe(IMousePositionHandler* subscriber);
protected:
    void DispatchEvent(double xPos, double yPos);
    std::vector<IMousePositionHandler*> subscribers_;

    static MousePositionManager* instance_;
};

