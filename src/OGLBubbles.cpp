#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "OGLBubbles.h"

#include <iostream>

// Current issue: exit code 0xc0000135
// If adding the library to the command line linker isn't your thing, uncomment this:
//#pragma comment(lib,"glfw3dll.lib")

using namespace std;

int main()
{
    // Initialize
    std::cout << "Hello World!";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    return 0;
}