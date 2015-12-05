#pragma once
class IMousePositionHandler
{
public:
    IMousePositionHandler();
    virtual ~IMousePositionHandler();
    virtual void HandleMousePosition(double xPos, double yPos) = 0;
};

