#pragma once
class IKeyboardKeyHandler
{
public:
    IKeyboardKeyHandler();
    virtual ~IKeyboardKeyHandler();
    virtual void HandleKeyboardKey(int key, int action, int modifier) = 0;
};

