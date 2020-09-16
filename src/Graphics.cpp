#ifndef GLFW_DLL
#define GLFW_DLL
#endif

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>

#include "Graphics.h"
#include "BubbleError.h"
#include <iostream>

using namespace std;

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
"uniform vec4 timeColor;\n"
"void main()\n"
"{\n"
"    PixelColor = vec4(0.9f, timeColor.y, 0.9f, 1.0f);\n"
"}\0";

Graphics::Graphics(GLFWwindow* wnd)
{
    Graphics::window = wnd;

    // Initializes shader array to the default max size
    maxSize = 6;
    shaders = new unsigned int [maxSize];
    VAOs = new unsigned int [maxSize];
    EBOs = new unsigned int [maxSize];
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

void Graphics::CreateShaders()
{
    // Shader generation (vertex)
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
        throw BubbleError(infoLog, 512, 0);
    }

    // Shader generation (pixel)
    unsigned int pixelShader;
    pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(pixelShader, 1, &pixelShaderSource, NULL);
    glCompileShader(pixelShader);

    glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(pixelShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PIXEL::COMPILATION_FAILED" << std::endl;
        throw BubbleError(infoLog, 512, 0);
    }

    // Creates shader program from the two compiled shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, pixelShader);
    glLinkProgram(shaderProgram);
    Graphics::shaders[0] = shaderProgram;

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED" << std::endl;
        throw BubbleError(infoLog, 512, 0);
    }

    // Every shader/rendering call from here on will use the compiled shader program
    glUseProgram(Graphics::shaders[0]);
    glDeleteShader(vertexShader);
    glDeleteShader(pixelShader);
}

void Graphics::GenerateRectangle()
{
    // Create vertices to be used
    float vertices[] = 
    {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = 
    {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // Creates and binds input buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    EBOs[0] = EBO;

    // Generates and binds a vertex buffer, then copies data to it
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specifies how OpenGL should interpret the buffer data (check above for matching details)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Adds all the defined data into a vertex array object (note that you can make an array of these)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs[1] = VAO;


    // Init process, should be same for any drawn object
    // 1. binds Vertex Array Object
    glBindVertexArray(VAOs[1]);
    // 2. copies the vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. sets the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Graphics::GenerateTriangle()
{
    // Create the vertices to be used
    float vertices[] = 
    {
        // x     y     z
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    /*Unused for current triangle
    // Input element buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // Binds buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    */

    // Generates and binds a vertex buffer, then copies data to it
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specifies how OpenGL should interpret the buffer data (check above for matching details)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Adds all the defined data into a vertex array object (note that you can make an array of these)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs[0] = VAO;


    // Init process, should be same for any drawn object
    // 1. binds Vertex Array Object
    glBindVertexArray(VAOs[0]);
    // 2. copies the vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. sets the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Graphics::DrawTriangle()
{
    // Send color data to the pixel shader c:
    // Note that if you want to keep this idea, change sin to dot product
    float time = glfwGetTime();
    float colorValue = (sin(time)/2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaders[0], "timeColor");

    // The final part of the process, this should be done in the render loop
    glUseProgram(shaders[0]);
    glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Graphics::DrawRectangle()
{
    // Send color data to the pixel shader c:
    // Note that if you want to keep this idea, change sin to dot product
    float time = glfwGetTime();
    float colorValue = (sin(time)/2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaders[0], "timeColor");

    // The final part of the process, this should be done in the render loop
    glUseProgram(shaders[0]);
    glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Graphics::SetMaxSize(int size)
{
    Graphics::maxSize = size;
    
    // TODO: Copy VAOs, EBOs and shader programs to new arrays of the updated size
}

void Graphics::Close()
{
    glfwTerminate();
}