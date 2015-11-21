#include "Window.h"
#include <iostream>
#include "Constant.h"
#include "RenderTree/Node/RenderTree.h"
#include "Water/WaveParticleManager.h"
#include "RenderTree/Scene.h"

#ifndef M_PI
#define M_PI 3.1415926535897932385f
#endif
#define DEG2RAD(a) (a / 180.0f) * (M_PI);

Scene* Window::scene_ = NULL;

Window::Window(std::string name, unsigned int width, unsigned int height, bool isFullscreen, bool isVSync)
	: name_(name)
	, width_(width)
	, height_(height)
	, isFullscreen_(isFullscreen)
	, isVSync_(isVSync)
	, monitor_(NULL)
	, window_(NULL)
{
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

bool Window::IsVSync()
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

void Window::Resize_CallBack(GLFWwindow* window, int width, int height)
{
    Constant::ViewportWidth = width;
    Constant::ViewPortHeight = height;
}

void Window::Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT || action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_RIGHT:
            Constant::theta -= DEG2RAD(2.0f);
            break;
        case GLFW_KEY_LEFT:
            Constant::theta += DEG2RAD(2.0f);
            break;
        case GLFW_KEY_UP:
            Constant::phi -= DEG2RAD(2.0f);
            break;
        case GLFW_KEY_DOWN:
            Constant::phi += DEG2RAD(2.0f);
            break;
        case GLFW_KEY_EQUAL:
            Constant::rho -= 0.5;
            break;
        case GLFW_KEY_MINUS:
            Constant::rho += 0.5;
            break;
        case GLFW_KEY_P:
			RenderTree::pause_ = !RenderTree::pause_;
            break;
        }
    }
}

void Window::Mouse_Callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_1:
			double xpos;
			double ypos;

			glfwGetCursorPos(window, &xpos, &ypos);
			glm::vec4 position(xpos, ypos, 0, 1);

			Camera* camera = scene_->GetCamera();
			glm::mat4 view(1);
			glm::mat4 projection(1);

			camera->Apply(view, projection);

			glm::mat4 transformationMatrix = glm::inverse(projection * view);
			position = transformationMatrix * position;

			glm::vec2 worldPosition(position.x / Constant::ViewportWidth, position.y / Constant::ViewPortHeight);

			WaveParticleManager::Instance()->SpawnCircularWave(10, worldPosition, 5.f, 0.1, 0.1);
            break;;
        }
	}
}

void Window::CreateWindow()
{
    if (isFullscreen_)
    {
        monitor_ = glfwGetPrimaryMonitor();
    }
   
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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

    glfwSetWindowSizeCallback(window_, Resize_CallBack);
    glfwSetKeyCallback(window_, Keyboard_Callback);
    glfwSetMouseButtonCallback(window_, Mouse_Callback);
}
