#pragma once
class IMouseButtonHandler
{
public:
    IMouseButtonHandler();
    virtual ~IMouseButtonHandler();
    virtual void HandleMouseKey(int button, int action, int modifier) = 0;
};

