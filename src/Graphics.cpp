#ifndef GLFW_DLL
#define GLFW_DLL
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Graphics.h"

Graphics::Graphics(GLFWwindow* wnd)
{
    window = wnd;
};

void Graphics::ClearBuffer(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::EndFrame()
{
    glfwSwapBuffers(window);
}

void Graphics::ProcessInput()
{
    // Add any additional inputs you want to be aware of in the following style
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Graphics::Close()
{
    glfwTerminate();
}