/**
 *  OGLBubbles
 *  A program that renders cool bubbles in a window!
 *  Note: If you want to work with textures, add the SAIL library: https://github.com/smoked-herring/sail
 *  @author Käthe Specht
 *  @version 1.0 - 09/14/2020
 */

#include "OGLBubbles.hpp"

#include <iostream>

#include "Graphics.hpp"
#include "Camera.hpp"
#include "Centroid.hpp"
#include "OGLBLOG.hpp"

Graphics*   Gfx;    // Global pointer to the graphics object
Camera*     Cam;    // Global pointer to the camera object
GLFWwindow* Window; // Global pointer to the window object

/**
 *  Entry point to the app, calls initialization functions and handles the render loop.
 */
int main()
{
    // Initializes GLFW window, shaders and graphics objects
    if ( !init() )
        return -1;
    
    // Loops while the window is open so graphics keep being drawn
    while ( !glfwWindowShouldClose(Window) )
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

            /**
             * // be sure to activate shader when setting uniforms/drawing objects
                lightingShader.use();
                lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
                lightingShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);

                // view/projection transformations
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                glm::mat4 view = camera.GetViewMatrix();
                lightingShader.setMat4("projection", projection);
                lightingShader.setMat4("view", view);

                // world transformation
                glm::mat4 model = glm::mat4(1.0f);
                lightingShader.setMat4("model", model);

                // render the cube
                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);


                // also draw the lamp object
                lightCubeShader.use();
                lightCubeShader.setMat4("projection", projection);
                lightCubeShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                model = glm::translate(model, lightPos);
                model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
                lightCubeShader.setMat4("model", model);

                glBindVertexArray(lightCubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
             */

            // Swap the front and back buffers and processes pending glfw events
            Gfx->EndFrame();
            glfwPollEvents();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            OGLBexit();
            return -1;
        }
    }

    OGLBexit();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_move_callback(GLFWwindow* window, double xPos, double yPos)
{
    Cam->ProcessMouse(xPos, yPos);
}

void OGLBexit()
{
    if ( Gfx != NULL )
        delete Gfx;
    if ( Cam != NULL )
        delete Cam;
    glfwTerminate();
}

void error_callback(int error_code, const char* description)
{
    std::cerr << description << std::endl;
}

bool init()
{
    // Initialize error callback, window & graphics
    glfwSetErrorCallback(error_callback);
    
    if ( glfwInit() == GLFW_FALSE )
    {
        std::cerr << "GLFW Initialization failed! Check include paths and library/dll locations." << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create the window and set it as the current context
    Window = glfwCreateWindow(800, 600, "OGLBubbles", NULL, NULL);
    if ( Window == NULL )
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(Window);

    if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) )
    {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return false;
    }

    // Set the default viewport size and initialize camera
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    Cam = new Camera(Window);
    if ( Cam == NULL )
    {
        std::cerr << "Failed to create camera object." << std::endl;
        glfwTerminate();
        return false;
    }

    // When the frame size changes, this calls a function to update it
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Capture: GLFW_CUROR_DISABLED
    glfwSetCursorPosCallback(Window, mouse_move_callback);

    // Creates the window's graphics object
    /*std::array<float,3> centerPoint;
    float radius;
    auto pair   = Center();
    centerPoint = pair.first;
    radius      = pair.second;
    radius *= 2.0f;
    Gfx = new Graphics(window, Cam, radius);*/

    Gfx = new Graphics(Window, Cam, 1.0f);
    if ( Gfx == NULL )
    {
        std::cerr << "Failed to create graphics object" << std::endl;
        delete Cam;
        glfwTerminate();
        return false;
    }
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Comment out for normal drawing

    // Loads reusable graphics
    try
    {
        Gfx->CreateShaders();
        Gfx->GenerateSphere(0);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        OGLBexit();
        return false;
    }

    return true;
}