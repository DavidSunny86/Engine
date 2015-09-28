#include "Window.h"
#include <iostream>

Window::Window(std::string name, unsigned int width, unsigned int height, bool isFullscreen, bool isVSync)
{
    name_ = name;
    width_ = width;
    height_ = height;
    isFullscreen_ = isFullscreen;
    isVSync_ = isVSync;
    monitor_ = NULL;
    window_ = NULL;
    CreateWindow();
}

Window::~Window()
{
}

void Window::SwapBuffer()
{
    glfwSwapBuffers(window_);
}

void Window::PollEvent()
{
    glfwPollEvents();
}

GLFWwindow* Window::GetWindow()
{
    return window_;
}

std::string Window::Name()
{
    return name_;
}

unsigned int Window::Width()
{
    return width_;
}

unsigned int Window::Height()
{
    return height_;
}

bool Window::IsFullscreen()
{
    return isFullscreen_;
}

bool Window::isVSync()
{
    return isVSync_;
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window_) != 0;
}

void Window::Name(const std::string & name)
{
    name_ = name;
}

void Window::Width(unsigned int width)
{
    width_ = width;
}

void Window::Height(unsigned int height)
{
    height_ = height;
}

void Window::IsFullScreen(bool isFullscreen)
{
    if (isFullscreen_ != isFullscreen)
    {
        monitor_ = NULL;
        isFullscreen_ = isFullscreen;
        glfwDestroyWindow(window_);
        CreateWindow();
    }
}

void Window::IsVSync(bool isVSync)
{
    if (isVSync_ != isVSync)
    {
        isVSync_ = isVSync;
        if (isVSync_)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }
    }
    
}

void Window::CreateWindow()
{
    if (isFullscreen_)
    {
        monitor_ = glfwGetPrimaryMonitor();
    }
   
    glfwWindowHint(GLFW_SAMPLES, 86);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width_, height_, name_.c_str(), monitor_, NULL);
    glfwMakeContextCurrent(window_);
    
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();
    if (isVSync_)
    {
        glfwSwapInterval(1);
    }
}
