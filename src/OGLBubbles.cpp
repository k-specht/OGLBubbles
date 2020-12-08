#ifndef GLFW_DLL
#define GLFW_DLL
#endif

#include "OGLBubbles.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>

#include "Graphics.hpp"
#include "Camera.hpp"
#include "Centroid.hpp"
#include "OGLBLOG.hpp"

/**
 *  OGLBubbles
 *  A program that renders cool bubbles in a window!
 *  TODO: Rearrange this file and add error handling.
 *  Note: If you want to work with textures, add the SAIL library: https://github.com/smoked-herring/sail
 *  @author Käthe Specht
 *  @version 1.0 - 09/14/2020
 */

Graphics* Gfx; // Global pointer to the graphics object
Camera*   Cam; // Global pointer to the camera object

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
 *  Cleans up any resources before exiting the program.
 */
void exit()
{
    if ( Gfx != NULL )
        delete Gfx;
    if ( Cam != NULL )
        delete Cam;
    glfwTerminate();
}

/**
 *  A callback function that handles GLFW errors.
 *  @param error_code  - A GLFW error code.
 *  @param description - A UTF-8 encoded string describing the error.
 */
void error_callback(int error_code, const char* description)
{
    std::cerr << description << std::endl;
}

/**
 *  Entry point to the app.
 *  This function initiates the window and attaches a Graphics object.
 *  Note that for the app to run, all dll files must be in the same directory as the exe file.
 */
int main()
{
    // Initialize error callback, window & graphics
    glfwSetErrorCallback(error_callback);

    GLFWwindow* window;
    if ( glfwInit() == GLFW_FALSE )
    {
        std::cerr << "GLFW Initialization failed! Check include paths and library/dll locations." << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create the window and set it as the current context
    window = glfwCreateWindow(800, 600, "OGLBubbles", NULL, NULL);
    if ( window == NULL )
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) )
    {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    // Set the default viewport size and initialize camera
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    Cam = new Camera(window);
    if ( Cam == NULL )
    {
        std::cerr << "Failed to create camera object." << std::endl;
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
    if ( Gfx == NULL )
    {
        std::cerr << "Failed to create graphics object" << std::endl;
        delete Cam;
        glfwTerminate();
        return -1;
    }
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Comment out for normal drawing

    // Loads reusable graphics
    try
    {
        Gfx->CreateShaders();
        Gfx->GenerateSphere(0);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit();
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

            // Physics - Incomplete, don't use
            //Gfx->CollisionCheck(Cam->GetX(), Cam->GetY(), Cam->GetMouseVelocity());
            //Gfx->RegenSphere(0);

            // Clear the back buffer before drawing to it
            Gfx->ClearBuffer(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Create transformations according to camera position
            Cam->UpdateCamera(); // Previously was at end of frame, check if this needs to be refactored
            Gfx->Transform(800.0f, 600.0f);
            Gfx->DrawSphere(0);

            // Swap the front and back buffers and processes pending glfw events
            Gfx->EndFrame();
            glfwPollEvents();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            exit();
            return -1;
        }
    }

    exit();
    return 0;
}