#ifndef GLFW_DLL
#define GLFW_DLL
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Graphics.h"
#include "BubbleError.h"

// Vertex Shader (update this when modifying VertexShader.GLSL)
const char *vertexShaderSource = "#version 420 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Pixel Shader (update this when modifying PixelShader.GLSL)
const char *pixelShaderSource = "#version 420 core\n"
"out vec4 PixelColor;\n"
"void main()\n"
"{\n"
"    PixelColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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

void Graphics::DrawTriangle()
{
    // Create the vertices to be used
    float vertices[] = 
    {
        // x     y     z
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Generates a buffer and assigns its unique id to VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Binds generated buffer to the array buffer (note that you can bind to multiple buffers of varying types)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copies the defined data into the specified buffer; static means it will be accessed a bunch without changing
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Makes a unique id for the shader and then creates it
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attaches the shader source code to be compiled at runtime (yeah it's a lil weird)
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Checks if that shader was correctly compiled
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        BubbleError("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", "Graphics.cpp", 81, infoLog);
    }

    
}

void Graphics::Close()
{
    glfwTerminate();
}