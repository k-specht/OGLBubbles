#define GLFW_DLL


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "OGLBubbles.h"

#include <iostream>

/**
 *  Updates the viewport to the new size of the window's frame.
 *  @param window - The window whose frame changed.
 *  @param width  - The width of the new frame size.
 *  @param height - The height of the new frame size.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/**
 *  Entry point to the app.
 *  Note that for the app to run, all dll files must be in the same directory as the exe file.
 */
int main()
{
    // Initialize
    std::cout << "Hello World!";
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.0);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window and sets it as the current context
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    // Sets the default viewport size
    glViewport(0, 0, 800, 600);

    // When the frame size changes, this calls a function to update it
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

    
    // Loops while the window is open so graphics keep being drawn
    while(!glfwWindowShouldClose(window))
    {
        // Swaps the front and back buffers
        glfwSwapBuffers(window);

        // Processes any pending events, like mouse movement
        glfwPollEvents();    
    }

    // Clean up all drawing resources before exiting the program
    glfwTerminate();
    return 0;
}