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
#include "Centroid.h"

// OpenGL Mathematics library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

Graphics::Graphics(GLFWwindow* wnd, Camera* cam, float radius)
{
    this->window = wnd;
    this->camera = cam;

    // Initializes shader array to the default max size
    maxSize = 6;
    shaders.resize(maxSize);
    VAOs = new unsigned int [maxSize];
    EBOs = new unsigned int [maxSize];
    sphere = new Sphere(radius);

    /*// Camera positions
    cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    view        = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);*/
};

void Graphics::GenerateCluster(int index)
{
    // The number of points in this cluster
    int n;

    // Read in points
    ifstream source;
    try
    {
        source.open("../OGLBubbles/bin/input.txt", ios::in);
    }
    catch (std::ios_base::failure &ex)
    {
        std::cout << ex.what() << std::endl;
    }

    if ( source.fail() ) 
    {
        std::cout << "Cannot open file: " << source.eof() << std::endl;
        return;
    }
    bool first = true;
    std::vector<float> vertices = vector<float>();
    
    for (string line; getline(source, line);)
    {
        istringstream in(line);

        if ( first )
        {
            in >> n;
            first = false;
            continue;
        }

        float p1, p2, p3;
        in >> p1;
        in >> p2;
        in >> p3;
        
        vertices.push_back(p1);
        vertices.push_back(p2);
        vertices.push_back(p3);
    }
    source.close();

    // Graphics Pipeline
    // Step 1: Create the data for the object and generate buffers

    // Generates and binds a vertex buffer, then copies data to it
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Adds all the defined data into a vertex array object (note that you can make an array of these)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs[index] = VAO;

    // Step 2. Copy the data into buffers and bind them to the current VAO
    glBindVertexArray(VAOs[index]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), static_cast<void*>(vertices.data()), GL_STATIC_DRAW);

    // Step 3. Set the vertex attribute pointers and enable them
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //std::cout << "Generation completed with size " << vertices.size() << ", expected: " << n * 3 << "." << std::endl;
}

void Graphics::DrawCluster(int index, int n)
{
    // Renders the cluster
    glUseProgram(shaders.back()->ID);
    glBindVertexArray(VAOs[index]);
    
    glDrawArrays(GL_LINE_LOOP, 0, n);
}

/*void Graphics::GenerateCentroid(int index, float center[3], float radius)
{
    // Graphics Pipeline
    // Step 1: Create the data for the object and generate buffers

    // Generates and binds a vertex buffer, then copies data to it
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Adds all the defined data into a vertex array object (note that you can make an array of these)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs[index] = VAO;

    // Step 2. Copy the data into buffers and bind them to the current VAO
    glBindVertexArray(VAOs[index]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), static_cast<void*>(vertices.data()), GL_STATIC_DRAW);

    // Step 3. Set the vertex attribute pointers and enable them
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Graphics::DrawCentroid(int index, float center[3], float radius)
{

}*/

void Graphics::ClearBuffer(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::EndFrame()
{
    glfwSwapBuffers(window);
}

/*
void Graphics::UpdateCamera()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}*/

/*void Graphics::ProcessInput()
{
    // Add any additional inputs you want to be aware of in the following style
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}*/

void Graphics::CreateShaders()
{
    shaders.push_back(new Shader(".\\shaders\\VertexShader.GLSL", ".\\shaders\\PixelShader.GLSL"));
    //shaders.back()->use();
}

void Graphics::UseShader()
{
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

void Graphics::GenerateCube(int index)
{
    float vertices[] = 
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
    };

    // Generates and binds a vertex buffer, then copies data to it
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Adds all the defined data into a vertex array object (note that you can make an array of these)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs[index] = VAO;

    // Input element buffer
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    //EBOs[index] = EBO;

    // Step 2. Copy the data into buffers and bind them to the current VAO
    glBindVertexArray(VAOs[index]);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Step 3. Set the vertex attribute pointers and enable them
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Graphics::GenerateSphere(int index)
{
    // Get icosahedron data from the sphere object
    std::vector <float>        vertices = sphere->GetVertices();
    std::vector <unsigned int> indices  = sphere->GetIndices();
    
    
    // Divide the icosahedron into a more spherical object
    sphere->Divide(1);

    unsigned int oldSize  = vertices.size();
    unsigned int oldSize2 = indices.size();
    vertices = sphere->GetVertices();
    indices  = sphere->GetIndices();

    std::cout << "New vertices size (was " << oldSize  / 3 << "): " << vertices.size() / 3 << std::endl;
    std::cout << "New indices size (was "  << oldSize2 / 3 << "): " << indices.size()  / 3 << std::endl;

    // Following is the typical OpenGL generation
    unsigned int VBO;
    glGenBuffers(1, &VBO);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), static_cast<void*>(indices.data()), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), static_cast<void*>(vertices.data()), GL_STATIC_DRAW);


    // Step 3. Set the vertex attribute pointers and enable them
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3* sizeof(float)));
    //glEnableVertexAttribArray(1);
}

void Graphics::DrawSphere(int index)
{
    // The final part of the process, this should be done in the render loop
    glUseProgram(shaders.back()->ID);
    //glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[index]);
    
    //glDrawArrays(GL_TRIANGLES, 0, 126);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[index]);
    glDrawElements(GL_TRIANGLES, sphere->GetIndices().size(), GL_UNSIGNED_INT, 0);
}

void Graphics::DrawCube(int index)
{
    // Step 4: Bind a VAO and draw the object

    // Send color data to the pixel shader c:
    // Note that if you want to keep this idea, change sin to dot product
    //float time = glfwGetTime();
    //float colorValue = (sin(time)/2.0f) + 0.5f;
    //int vertexColorLocation = glGetUniformLocation(shaders.back()->ID, "timeColor");

    // The final part of the process, this should be done in the render loop
    glUseProgram(shaders.back()->ID);
    //glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[index]);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[index]);
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void Graphics::DrawTriangle(int index)
{
    // Step 4: Bind a VAO and draw the object

    // Send color data to the pixel shader c:
    // Note that if you want to keep this idea, change sin to dot product
    float time              = glfwGetTime();
    float colorValue        = (sin(time)/2.0f) + 0.5f;
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

void Graphics::Transform(float width, float height)
{
    /*// Initializes matrix to identity and translates it according to time
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));

    // get matrix's uniform location and set matrix
    UseShader();
    unsigned int transformLoc = glGetUniformLocation(shaders.back()->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    */
    // create transformations
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    model      = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));//(float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    //view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shaders.back()->ID, "model");
    unsigned int viewLoc  = glGetUniformLocation(shaders.back()->ID, "view" );

    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)   );
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, &camera->GetView()[0][0]);
    
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    glUniformMatrix4fv(glGetUniformLocation(shaders.back()->ID, "projection"), 1, GL_FALSE, &projection[0][0]);
}

void Graphics::SetMaxSize(int size)
{
    this->maxSize = size;
    
    // TODO: Copy VAOs, EBOs and shader programs to new arrays of the updated size
}

void Graphics::Mouse(GLFWwindow* window, double xpos, double ypos)
{

}

void Graphics::Close()
{
    glfwTerminate();
    if (sphere != NULL)
        delete sphere;
    for (auto shader : shaders) if (shader != NULL) delete shader;
    delete VAOs;   // Handled by glfwTerminate?
    delete EBOs;   // Handled by glfwTerminate?
    //delete camera; // handled by OGLBubbles
    //delete window; // handled by glfwTerminate
}