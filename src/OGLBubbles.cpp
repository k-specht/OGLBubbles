#ifndef GLFW_DLL
#define GLFW_DLL
#endif


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "OGLBubbles.h"
#include "Graphics.h"
#include "Camera.h"
#include "Centroid.h"
#include "BubbleError.h"

#include <iostream>

/**
 *  OGLBubbles
 *  A program that generates cool bubbles to a window!
 *  TODO Rearrange this messy, messy file.
 *  Note: If you want to work with textures, add the SAIL library: https://github.com/smoked-herring/sail
 *  @author Käthe Specht
 *  @version 1.0 - 09/14/2020
 */

// Global pointer to the graphics object
Graphics* Gfx;
Camera*   Cam;

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
 *  Updates the viewport to the new size of the window's frame.
 *  @param window - The window whose frame changed.
 *  @param xPos   - The new x position of the mouse.
 *  @param yPos   - The new y position of the mouse.
 */
void mouse_move_callback(GLFWwindow* window, double xPos, double yPos)
{
    Cam->ProcessMouse(xPos, yPos);
}

/**
 *  Entry point to the app.
 *  This function initiates the window and attaches a Graphics object.
 *  Note that for the app to run, all dll files must be in the same directory as the exe file.
 */
int main()
{
    // Initialize window & graphics
    GLFWwindow* window;
    try
    {
        if(glfwInit() == GLFW_FALSE)
        {
            std::cout << "GLFW Initialization failed! Check include paths and library/dll locations." << std::endl;
            return -1;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        // Create the window and sets it as the current context
        window = glfwCreateWindow(800, 600, "OGLBubbles", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        // Sets the default viewport size
        glViewport(0, 0, 800, 600);
        glEnable(GL_DEPTH_TEST);
        Cam = new Camera(window);
        if (Cam == NULL)
        {
            std::cerr << "Failed to create camera object" << std::endl;
            glfwTerminate();
            return -1;
        }

        // When the frame size changes, this calls a function to update it
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Capture: GLFW_CUROR_DISABLED
        glfwSetCursorPosCallback(window, mouse_move_callback);

        // Creates the window's graphics object
        /*std::array<float,3> centerPoint;
        float radius;
        auto pair   = Center();
        centerPoint = pair.first;
        radius      = pair.second;
        radius *= 2.0f;
        Gfx = new Graphics(window, Cam, radius);*/
        Gfx = new Graphics(window, Cam, 1.0f);
        if (Gfx == NULL)
        {
            std::cerr << "Failed to create graphics object" << std::endl;
            glfwTerminate();
            return -1;
        }
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Comment out for normal drawing
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    // Loads reusable graphics
    try
    {
        Gfx->CreateShaders();
        //Gfx->GenerateCluster(0);
        //std::cout << "center: {" << centerPoint[0] << ", " << centerPoint[1] << ", " << centerPoint[2] << "}, radius: " << radius << std::endl;
        //Gfx->GenerateTriangle(0);
        
        Gfx->GenerateSphere(0);
        //Gfx->GenerateCube(1);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    
    // Loops while the window is open so graphics keep being drawn
    while ( !glfwWindowShouldClose(window) )
    {
        // Draw things in the render loop! 
        // Note that the thrown exceptions are caught so that the debug window prints related info
        try
        {
            // Process any inputs
            Cam->ProcessInput();

            // Clears the buffer to the specified color (aka, background color)
            Gfx->ClearBuffer(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // create transformations
            Gfx->Transform(800.0f, 600.0f);

            //Gfx->DrawCluster(0,5);
            //Gfx->DrawTriangle(0);
            //Gfx->DrawCube(1);
            Gfx->DrawSphere(0);

            Cam->UpdateCamera();

            // Swaps the front and back buffers
            Gfx->EndFrame();

            // Processes any pending events in the queue
            glfwPollEvents();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return -1;
        }
    }

    // Clean up all drawing resources before exiting the program
    try 
    {
        delete Gfx;
        delete Cam;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    
    //delete window;
    return 0;
}