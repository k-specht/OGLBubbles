//#include "/lib/GLFW/glfw3.h"
//#pragma once

#ifndef GLFW_DLL
#define GLFW_DLL
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

/**
 *  Updates the viewport to the new size of the window's frame.
 *  @param window - The window whose frame changed.
 *  @param width  - The width of the new frame size.
 *  @param height - The height of the new frame size.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/**
 *  Updates the viewport to the new size of the window's frame.
 *  @param window - The window whose frame changed.
 *  @param xPos   - The new x position of the mouse.
 *  @param yPos   - The new y position of the mouse.
 */
void mouse_move_callback(GLFWwindow* window, double xPos, double yPos);

/**
 *  Cleans up any resources before exiting the program.
 */
void OGLBexit();

/**
 *  A callback function that handles GLFW errors.
 *  @param error_code  - A GLFW error code.
 *  @param description - A UTF-8 encoded string describing the error.
 */
void error_callback(int error_code, const char* description);

/**
 *  Initializes the OpenGL/GLFW window, graphics and camera objects.
 *  This function handles shader creation and drawable object generation as well.
 */
bool init();