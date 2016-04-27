#pragma once
#include <GLFW/glfw3.h>
enum Keys {
    Space = 0, Apostrophe, Comma, Minus, Period, Slash, Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Semicolon, Equal, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Left_Bracket, Right_Bracket, Grave_Accent, World1, World2, Escape, Enter, Tab, Backspace, Insert, Delete, Right, Left, Up, Down, Page_Up, Page_Down, Home, End, Caps_Lock, Scroll_Lock, Num_Lock, Print_Screen, Pause, F1,
    F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25, KP0, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9, Decimal, Divide, Multiply, Subtract, Add, KPEnter, Left_Shift,
    Left_Control, Left_Alt, Left_Super, Right_Shift, Right_Control, Right_Alt, Right_Super, Menu, Number_Of_Keys
};

enum MouseButton {
    Button_1 = 0,
    Button_2,
    Button_3,
    Button_4,
    Button_5,
    Button_6,
    Button_7,
    Button_8,
    Number_Of_Buttons
};

enum KeyState { Released, FirstPress, Press };

struct MousePosition {
    double x;
    double y;
};

class Input
{
public:
    static void UpdateInputs(GLFWwindow* window);
    static KeyState GetKeyState(Keys key);
    static MousePosition GetMousePosition();
    static KeyState GetMouseButtonState(MouseButton button);
private:
    static void UpdateKey(GLFWwindow* window, Keys key, int glfwDefine);
    static void UpdateMouseButton(GLFWwindow* window, MouseButton key, int glfwDefine);
    static KeyState m_keysState[Number_Of_Keys];
    static KeyState m_mouseButtonState[Number_Of_Buttons];
    static MousePosition m_mousePosition;
};

