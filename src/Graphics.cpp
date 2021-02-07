#ifndef GLFW_DLL
#define GLFW_DLL
#endif

#include "Graphics.hpp"

// std libraries
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ios>

// OpenGL Mathematics library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>

// Local libraries
#include "OGLBLOG.hpp"
#include "Shader.hpp"
#include "Centroid.hpp"

Graphics::Graphics(GLFWwindow* wnd, Camera* cam, float radius)
{
    this->window = wnd;
    this->camera = cam;

    // Initializes shader array to the default max size
    maxSize = 6;
    VAOs = new unsigned int [maxSize];
    VBOs = new unsigned int [maxSize];
    EBOs = new unsigned int [maxSize];
    sphere = new Sphere(radius);
}

void Graphics::GenerateCluster(int index)
{
    // The number of points in this cluster
    int n;

    // Read in points
    std::ifstream source;
    try
    {
        source.open("../OGLBubbles/bin/input.txt", std::ios::in);
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
    std::vector<float> vertices = std::vector<float>();
    
    // Iterates over the input
    for ( std::string line; std::getline(source, line); )
    {
        std::istringstream in(line);

        if ( first )
        {
            in >> n;
            first = false;
            continue;
        }

        float p1;
        float p2;
        float p3;
        
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
}

void Graphics::DrawCluster(int index, int n)
{
    // Renders the cluster
    glUseProgram(shaders.back()->ID);
    glBindVertexArray(VAOs[index]);
    
    glDrawArrays(GL_LINE_LOOP, 0, n);
}

void Graphics::ClearBuffer(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::EndFrame()
{
    glfwSwapBuffers(window);
}

void Graphics::CreateShaders()
{
    shaders.push_back(new Shader("..\\shaders\\LightVS.GLSL", "..\\shaders\\LightPS.GLSL"));

    shaders.push_back(new Shader("..\\shaders\\VertexShader.GLSL", "..\\shaders\\PixelShader.GLSL"));
    glUniform3f(glGetUniformLocation(shaders[1]->ID, "objectColor"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaders[1]->ID, "lightColor" ), 1.0f, 1.0f, 1.0f );
    //glUniform3f(glGetUniformLocation(shaders[1]->ID, "lightPos" ), lightPos[0], lightPos[1], lightPos[2] );
    glUniform3f(glGetUniformLocation(shaders[1]->ID, "viewPos" ), camera->GetCameraPos().x, camera->GetCameraPos().y, camera->GetCameraPos().z);
}

void Graphics::UseShader(int shaderID)
{
    shaders[shaderID]->use();
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
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Step 3. Set the vertex attribute pointers and enable them
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
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f ,  0.0f, 1.0f, 0.0f
    };

    // Generates and binds a vertex buffer, then copies data to it
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Adds all the defined data into a vertex array object (note that you can make an array of these)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs[index] = VAO;

    // Step 2. Copy the data into buffers and bind them to the current VAO
    glBindVertexArray(VAOs[index]);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) / sizeof(indices[0]), static_cast<void*>(indices), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), static_cast<void*>(vertices), GL_STATIC_DRAW);


    // Step 3. Set the vertex attribute pointers and enable them
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Graphics::GenerateSphere(int index)
{
    // Get icosahedron data from the sphere object
    std::vector <float>        vertices;// = sphere->GetVertices();
    //std::vector <float>        normals;//  = sphere->GetNormals();
    std::vector <unsigned int> indices;//  = sphere->GetIndices();
    
    // Divide the icosahedron into a more spherical object
    sphere->Divide(2);
    sphere->GenerateNormals();

    vertices = sphere->GetVertNorms();
    indices  = sphere->GetIndices();

    // Graphics Pipeline Step 1: Generate buffers & vertex/index arrays
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    VBOs[index] = VBO;

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    VAOs[index] = VAO;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Graphics::RegenSphere(int index)
{
    std::vector <float> vertices = sphere->GetVertices();

    glBindVertexArray(VAOs[index]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[index]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), static_cast<void*>(vertices.data()), GL_STATIC_DRAW);
}

void Graphics::DrawSphere(int index, int shaderID)
{
    // The final part of the process, this should be done in the render loop
    glUseProgram(shaders[shaderID]->ID);
    //glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[index]);
    
    //glDrawArrays(GL_TRIANGLES, 0, 126);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[index]);
    glDrawElements(GL_TRIANGLES, sphere->GetIndices().size(), GL_UNSIGNED_INT, 0);
}

void Graphics::DrawCube(int index, int shaderID)
{
    // Step 4: Bind a VAO and draw the object
    // The final part of the process, this should be done in the render loop
    glUseProgram(shaders[shaderID]->ID);
    //glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[index]);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

void Graphics::DrawTriangle(int index, int shaderID)
{
    // Step 4: Bind a VAO and draw the object

    // Send color data to the pixel shader c:
    // Note that if you want to keep this idea, change sin to dot product
    float time              = glfwGetTime();
    float colorValue        = (sin(time)/2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaders[shaderID]->ID, "timeColor");

    // The final part of the process, this should be done in the render loop
    glUseProgram(shaders[shaderID]->ID);
    glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[index]);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Graphics::DrawRectangle(int index, int shaderID)
{
    // Step 4. Bind a VAO and draw the object

    // Send color data to the pixel shader c:
    // Note that if you want to keep this idea, change sin to dot product
    float time = glfwGetTime();
    float colorValue = (sin(time)/2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaders[shaderID]->ID, "timeColor");

    // The final part of the process, this should be done in the render loop
    glUseProgram(shaders[shaderID]->ID);
    glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[index]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[index]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Graphics::Transform(float width, float height, int shaderID)
{
    // create transformations
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    model      = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));//(float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    //view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

    // Retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shaders[shaderID]->ID, "model");
    unsigned int viewLoc  = glGetUniformLocation(shaders[shaderID]->ID, "view" );
    unsigned int lightLoc = glGetUniformLocation(shaders[shaderID]->ID, "light" );

    // Pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)    );
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, &camera->GetView()[0][0] );
    glm::vec3 light = camera->GetLightPos();
    glUniform3f(lightLoc, light.x, light.y, light.z);
    //glUniformMatrix4fv(lightLoc, 1, GL_FALSE, &lightPos[0] );
    
    // Note: Projection matrix rarely changes, refactor this outside of the main rendering loop
    glUniformMatrix4fv(glGetUniformLocation(shaders[shaderID]->ID, "projection"), 1, GL_FALSE, &projection[0][0]);
}

void Graphics::TransformLight(float width, float height, int shaderID)
{
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    
    model      = glm::translate(model, camera->GetLightPos());
    model      = glm::scale(model, glm::vec3(0.1f));
    projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

    // Retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shaders[shaderID]->ID, "model");
    unsigned int viewLoc  = glGetUniformLocation(shaders[shaderID]->ID, "view" );
    

    // Pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)   );
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, &camera->GetView()[0][0]);
    
    
    // Note: Projection matrix rarely changes, refactor this outside of the main rendering loop
    glUniformMatrix4fv(glGetUniformLocation(shaders[shaderID]->ID, "projection"), 1, GL_FALSE, &projection[0][0]);
}

void Graphics::SetMaxSize(int size)
{
    this->maxSize = size;
    
    // TODO: Copy VAOs, EBOs and shader programs to new arrays of the updated size
}

void Graphics::Mouse(GLFWwindow* window, double xpos, double ypos)
{
    // Stub
}

void Graphics::Collision(std::array<float,3> vertex, float magnitude)
{
    // Stub
    sphere->Collision(vertex, magnitude);
}

void Graphics::CollisionCheck(float x, float y, float velocity)
{
    // Get rough sphere bounds
    // Model center! {0,0}!!! need to project this into world space
    std::array<float,3> center = sphere->FindCenter();
    float radius = sphere->GetRadius();

    // Transform model center into worldspace
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // TODO: Multiply center point by model -> world space transformation
    

    // TODO: Transform x, y position into screen percentage

    // TODO: Transform screen percentage into view space coordinates

    // Check if x,y is within sphere
    if ( (x != 0.0f && y != 0.0f) && (((x - center[0]) * (x - center[0]) + (y - center[1]) * (y - center[1]) /*+ (z - center[2]) * (z - center[2])*/ ) < radius * radius) )
    {
        std::cout << "Collision detected: {" << x << ", " << y << "}, velocity {" << velocity << "}." << std::endl;
        Collision({x, y, center[2]}, velocity);
    }
    else 
    {
        std::cout << "No collision detected, distance: {x: " << x << ", " << center[0] << "},  {y: " << y << ", " << center[1] << "}." << std::endl;
    }

    /*// Old code, delete later (it doesn't even check the triangles...)
    std::vector<float> vertices = sphere->GetVertices();
    for (int i = 0; i < vertices.size(); i += 3)
    {
        if (vertices[i] == x && vertices[i + 1] == y && !(x == 0 && y == 0))
        {
            std::cout << "Collision detected: [" << x << ", " << y << "]." << std::endl;
            Collision({vertices[i], vertices[i + 1], vertices[i + 2] }, 0.05f);
            break;
        }
    }*/
}

void Graphics::Close()
{
    glfwTerminate();

    // Delete sphere object
    if ( sphere != NULL )
        delete sphere;

    // Delete shaders (glfwTerminate might already handle this...)
    for (auto shader : shaders)
        if (shader != NULL)
            delete shader;
}
