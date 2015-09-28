#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "IL\il.h"

#include "Engine.h"
int main()
{
    glfwInit();
    ilInit();

    Engine* e = new Engine();
    e->Loop();
    delete e;
    return 0;
}