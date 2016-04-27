#include "Input.h"
KeyState Input::m_keysState[Number_Of_Keys] = { Released };
KeyState Input::m_mouseButtonState[Number_Of_Buttons] = { Released };
MousePosition Input::m_mousePosition = { 0.0,0.0 };
void Input::UpdateInputs(GLFWwindow* window)
{
    //Mouse
    glfwGetCursorPos(window, &m_mousePosition.x, &m_mousePosition.y);
    UpdateMouseButton(window, Button_1, GLFW_MOUSE_BUTTON_1);
    UpdateMouseButton(window, Button_2, GLFW_MOUSE_BUTTON_2);
    UpdateMouseButton(window, Button_3, GLFW_MOUSE_BUTTON_3);
    UpdateMouseButton(window, Button_4, GLFW_MOUSE_BUTTON_4);
    UpdateMouseButton(window, Button_5, GLFW_MOUSE_BUTTON_5);
    UpdateMouseButton(window, Button_6, GLFW_MOUSE_BUTTON_6);
    UpdateMouseButton(window, Button_7, GLFW_MOUSE_BUTTON_7);
    UpdateMouseButton(window, Button_8, GLFW_MOUSE_BUTTON_8);

    //Keyboards
    UpdateKey(window, Space, GLFW_KEY_SPACE);
    UpdateKey(window, Apostrophe, GLFW_KEY_APOSTROPHE);
    UpdateKey(window, Comma, GLFW_KEY_COMMA);
    UpdateKey(window, Minus, GLFW_KEY_MINUS);
    UpdateKey(window, Period, GLFW_KEY_PERIOD);
    UpdateKey(window, Slash, GLFW_KEY_SLASH);
    UpdateKey(window, Zero, GLFW_KEY_0);
    UpdateKey(window, One, GLFW_KEY_1);
    UpdateKey(window, Two, GLFW_KEY_2);
    UpdateKey(window, Three, GLFW_KEY_3);
    UpdateKey(window, Four, GLFW_KEY_4);
    UpdateKey(window, Five, GLFW_KEY_5);
    UpdateKey(window, Six, GLFW_KEY_6);
    UpdateKey(window, Seven, GLFW_KEY_7);
    UpdateKey(window, Eight, GLFW_KEY_8);
    UpdateKey(window, Nine, GLFW_KEY_9);
    UpdateKey(window, Semicolon, GLFW_KEY_SEMICOLON);
    UpdateKey(window, Equal, GLFW_KEY_EQUAL);

    UpdateKey(window, A, GLFW_KEY_A);
    UpdateKey(window, B, GLFW_KEY_B);
    UpdateKey(window, C, GLFW_KEY_C);
    UpdateKey(window, D, GLFW_KEY_D);
    UpdateKey(window, E, GLFW_KEY_E);
    UpdateKey(window, F, GLFW_KEY_F);
    UpdateKey(window, G, GLFW_KEY_G);
    UpdateKey(window, H, GLFW_KEY_H);
    UpdateKey(window, I, GLFW_KEY_I);
    UpdateKey(window, J, GLFW_KEY_J);
    UpdateKey(window, K, GLFW_KEY_K);
    UpdateKey(window, L, GLFW_KEY_L);
    UpdateKey(window, M, GLFW_KEY_M);
    UpdateKey(window, N, GLFW_KEY_N);
    UpdateKey(window, O, GLFW_KEY_O);
    UpdateKey(window, P, GLFW_KEY_P);
    UpdateKey(window, Q, GLFW_KEY_Q);
    UpdateKey(window, R, GLFW_KEY_R);
    UpdateKey(window, S, GLFW_KEY_S);
    UpdateKey(window, T, GLFW_KEY_T);
    UpdateKey(window, U, GLFW_KEY_U);
    UpdateKey(window, V, GLFW_KEY_V);
    UpdateKey(window, W, GLFW_KEY_W);
    UpdateKey(window, X, GLFW_KEY_X);
    UpdateKey(window, Y, GLFW_KEY_Y);
    UpdateKey(window, Z, GLFW_KEY_Z);

    UpdateKey(window, Left_Bracket, GLFW_KEY_LEFT_BRACKET);
    UpdateKey(window, Right_Bracket, GLFW_KEY_RIGHT_BRACKET);
    UpdateKey(window, Grave_Accent, GLFW_KEY_RIGHT_BRACKET);
    UpdateKey(window, World1 , GLFW_KEY_WORLD_1);
    UpdateKey(window, World2, GLFW_KEY_WORLD_2);
    UpdateKey(window, Escape, GLFW_KEY_ESCAPE);
    UpdateKey(window, Enter, GLFW_KEY_ENTER);
    UpdateKey(window, Tab, GLFW_KEY_TAB);
    UpdateKey(window, Backspace, GLFW_KEY_BACKSPACE);
    UpdateKey(window, Insert, GLFW_KEY_INSERT);
    UpdateKey(window, Delete, GLFW_KEY_DELETE);
    UpdateKey(window, Right, GLFW_KEY_RIGHT);
    UpdateKey(window, Left, GLFW_KEY_LEFT);
    UpdateKey(window, Up, GLFW_KEY_UP);
    UpdateKey(window, Down, GLFW_KEY_DOWN);
    UpdateKey(window, Page_Up, GLFW_KEY_PAGE_UP);
    UpdateKey(window, Page_Down, GLFW_KEY_PAGE_DOWN);
    UpdateKey(window, Home, GLFW_KEY_HOME);
    UpdateKey(window, End, GLFW_KEY_END);
    UpdateKey(window, Caps_Lock, GLFW_KEY_CAPS_LOCK);
    UpdateKey(window, Scroll_Lock, GLFW_KEY_SCROLL_LOCK);
    UpdateKey(window, Num_Lock, GLFW_KEY_NUM_LOCK);
    UpdateKey(window, Print_Screen, GLFW_KEY_PRINT_SCREEN);
    UpdateKey(window, Pause, GLFW_KEY_PAUSE);
    
    UpdateKey(window, F1, GLFW_KEY_F1);
    UpdateKey(window, F2, GLFW_KEY_F2);
    UpdateKey(window, F3, GLFW_KEY_F3);
    UpdateKey(window, F4, GLFW_KEY_F4);
    UpdateKey(window, F5, GLFW_KEY_F5);
    UpdateKey(window, F6, GLFW_KEY_F6);
    UpdateKey(window, F7, GLFW_KEY_F7);
    UpdateKey(window, F8, GLFW_KEY_F8);
    UpdateKey(window, F9, GLFW_KEY_F9);
    UpdateKey(window, F10, GLFW_KEY_F10);
    UpdateKey(window, F11, GLFW_KEY_F11);
    UpdateKey(window, F12, GLFW_KEY_F12);
    UpdateKey(window, F13, GLFW_KEY_F13);
    UpdateKey(window, F14, GLFW_KEY_F14);
    UpdateKey(window, F15, GLFW_KEY_F15);
    UpdateKey(window, F16, GLFW_KEY_F16);
    UpdateKey(window, F17, GLFW_KEY_F17);
    UpdateKey(window, F18, GLFW_KEY_F18);
    UpdateKey(window, F19, GLFW_KEY_F19);
    UpdateKey(window, F20, GLFW_KEY_F20);
    UpdateKey(window, F21, GLFW_KEY_F21);
    UpdateKey(window, F22, GLFW_KEY_F22);
    UpdateKey(window, F23, GLFW_KEY_F23);
    UpdateKey(window, F24, GLFW_KEY_F24);
    UpdateKey(window, F25, GLFW_KEY_F25);

    UpdateKey(window, KP0, GLFW_KEY_KP_0);
    UpdateKey(window, KP1, GLFW_KEY_KP_1);
    UpdateKey(window, KP2, GLFW_KEY_KP_2);
    UpdateKey(window, KP3, GLFW_KEY_KP_3);
    UpdateKey(window, KP4, GLFW_KEY_KP_4);
    UpdateKey(window, KP5, GLFW_KEY_KP_5);
    UpdateKey(window, KP6, GLFW_KEY_KP_6);
    UpdateKey(window, KP7, GLFW_KEY_KP_7);
    UpdateKey(window, KP8, GLFW_KEY_KP_8);
    UpdateKey(window, KP9, GLFW_KEY_KP_9);

    UpdateKey(window, Decimal, GLFW_KEY_KP_DECIMAL);
    UpdateKey(window, Divide, GLFW_KEY_KP_DIVIDE);
    UpdateKey(window, Multiply, GLFW_KEY_KP_MULTIPLY);
    UpdateKey(window, Subtract, GLFW_KEY_KP_SUBTRACT);
    UpdateKey(window, Add, GLFW_KEY_KP_ADD);
    UpdateKey(window, KPEnter, GLFW_KEY_KP_ENTER);
    UpdateKey(window, Left_Shift, GLFW_KEY_LEFT_SHIFT);
    UpdateKey(window, Left_Control, GLFW_KEY_LEFT_CONTROL);
    UpdateKey(window, Left_Alt, GLFW_KEY_LEFT_ALT);
    UpdateKey(window, Left_Super, GLFW_KEY_LEFT_SUPER);

    UpdateKey(window, Right_Shift, GLFW_KEY_RIGHT_SHIFT);
    UpdateKey(window, Right_Control, GLFW_KEY_RIGHT_CONTROL);
    UpdateKey(window, Right_Alt, GLFW_KEY_RIGHT_ALT);
    UpdateKey(window, Right_Super, GLFW_KEY_RIGHT_SUPER);
    UpdateKey(window, Menu, GLFW_KEY_MENU);

}

KeyState Input::GetKeyState(Keys key)
{
    return m_keysState[key];
}

MousePosition Input::GetMousePosition()
{
    return m_mousePosition;
}

KeyState Input::GetMouseButtonState(MouseButton button)
{
    return m_mouseButtonState[button];
}

void Input::UpdateKey(GLFWwindow* window, Keys key, int glfwDefine)
{
    int keyState = glfwGetKey(window, glfwDefine);
    if (keyState == GLFW_PRESS)
    {
        m_keysState[key] = m_keysState[key] == KeyState::FirstPress ? Press : FirstPress;
    }
    else
    {
        m_keysState[key] = Released;
    }
}

void Input::UpdateMouseButton(GLFWwindow* window, MouseButton button, int glfwDefine)
{
    int keyState = glfwGetMouseButton(window, glfwDefine);
    if (keyState == GLFW_PRESS)
    {
        m_mouseButtonState[button] = m_mouseButtonState[button] == KeyState::FirstPress ? Press : FirstPress;
    }
    else
    {
        m_mouseButtonState[button] = Released;
    }
}
