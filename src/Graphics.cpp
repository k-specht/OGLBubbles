#ifndef GLFW_DLL
#define GLFW_DLL
#endif

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>

#include "Graphics.h"
#include "BubbleError.h"
#include <iostream>
#include "Shader.h"
#include <vector>

using namespace std;

Graphics::Graphics(GLFWwindow* wnd)
{
    Graphics::window = wnd;

    // Initializes shader array to the default max size
    maxSize = 6;
    shaders.resize(maxSize);
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
    shaders.push_back(new Shader(".\\shaders\\VertexShader.GLSL", ".\\shaders\\PixelShader.GLSL"));
    shaders.back()->use();
}

void Graphics::GenerateRectangle(int index)
{
    // Step 1. Create the object data and generate buffers
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

    // Generates and binds a vertex buffer, then copies data to it
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Adds all the defined data into a vertex array object (note that you can make an array of these)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs[index] = VAO;

    // Input element buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    EBOs[index] = EBO;

    // Step 2. Copy the data into buffers and bind them to the current VAO
    glBindVertexArray(VAOs[index]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Step 3. Set the vertex attribute pointers and enable them
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Graphics::GenerateTriangle(int index)
{
    // Step 1: Create the data for the object and generate buffers
    float vertices[] = 
    {
        // x     y     z     r     g     b
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = 
    {
        0, 1, 2
    };

    // Generates and binds a vertex buffer, then copies data to it
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Adds all the defined data into a vertex array object (note that you can make an array of these)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs[index] = VAO;

    // Input element buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    EBOs[index] = EBO;

    // Step 2. Copy the data into buffers and bind them to the current VAO
    glBindVertexArray(VAOs[index]);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Step 3. Set the vertex attribute pointers and enable them
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Graphics::DrawTriangle(int index)
{
    // Step 4: Bind a VAO and draw the object

    // Send color data to the pixel shader c:
    // Note that if you want to keep this idea, change sin to dot product
    float time = glfwGetTime();
    float colorValue = (sin(time)/2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaders.back()->ID, "timeColor");

    // The final part of the process, this should be done in the render loop
    glUseProgram(shaders.back()->ID);
    glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[index]);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[index]);
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void Graphics::DrawRectangle(int index)
{
    // Step 4. Bind a VAO and draw the object

    // Send color data to the pixel shader c:
    // Note that if you want to keep this idea, change sin to dot product
    float time = glfwGetTime();
    float colorValue = (sin(time)/2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaders.back()->ID, "timeColor");

    // The final part of the process, this should be done in the render loop
    glUseProgram(shaders.back()->ID);
    glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[index]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[index]);
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