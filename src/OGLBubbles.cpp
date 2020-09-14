#define GLFW_DLL
#define GLFW_INCLUDE_NONE

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"
#include "OGLBubbles.h"

#include <iostream>

#pragma comment(lib,".\\lib\\GLFW\\glfw3dll.lib")

using namespace std;

int main()
{
    std::cout << "Hello World!";
    //glfwInit();
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
    return 0;
}