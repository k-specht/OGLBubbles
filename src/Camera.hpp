#ifndef CAMERA
#define CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OGLBLOG.hpp"

/**
 *  A header class for handling the viewport and input.
 */
class Camera
{
    public:
        Camera(GLFWwindow* win)
        {
            window = win;

            // Camera positions
            cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
            cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
            view        = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            yaw      = 0.0f;
            pitch    = 0.0f;
            lastPosX = 0.0f;
            lastPosY = 0.0f;
        };
        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;
        ~Camera() { }; // Deconstructor

        /**
         *  Processes the movement of the mouse.
         *  TODO: Change mouse to non-captured, do something else with mouse data.
         *  @param xPos - The new x position of the mouse.
         *  @param yPos - The new y position of the mouse.
         */
        void ProcessMouse(double xPos, double yPos)
        {
            /*if (firstMouse)
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }*/

            //float xOff = xPos     - lastPosX;
            //float yOff = lastPosY - yPos;    // reversed since y-coordinates go from bottom to top

            // Calculate the change in distance since the last frame (note: time here is dependent upon framerate...)
            velocity = std::abs(std::sqrt(lastPosX * lastPosX + lastPosY * lastPosY) - std::sqrt( xPos * xPos + yPos * yPos));

            lastPosX = xPos;
            lastPosY = yPos;

            /*float sensitivity = 0.1f; // TODO: Create Camera default sensitivity
            xOff *= sensitivity;
            yOff *= sensitivity;

            yaw   += xOff;
            pitch += yOff;

            // Avoids unexpected behavior around edges of camera movement
            if(pitch > 89.0f)
                pitch = 89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;

            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw) ) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw) ) * cos(glm::radians(pitch));
            //cameraFront = glm::normalize(direction);*/
        };

        /**
         *  Processes the GLFW inputs and applies them to the viewport.
         */
        void ProcessInput()
        {
            // Add any additional inputs you want to be aware of in the following style
            if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            // Camera xyz movement
            const float cameraSpeed = 0.05f; // TODO: Add default camera speed values
            const float viewSpeed   = 0.8f;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                cameraPos += cameraSpeed * cameraFront;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                cameraPos -= cameraSpeed * cameraFront;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

            // Camera rotational movement
            if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS)
                yaw   -= viewSpeed;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                yaw   += viewSpeed;
            if (glfwGetKey(window, GLFW_KEY_UP   ) == GLFW_PRESS)
                pitch += viewSpeed;
            if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS)
                pitch -= viewSpeed;

            // Camera rotational fix for edge behavior
            if (pitch >  89.0f)
                pitch =  89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            // Updates viewport based on changes in pitch/yaw
            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw) ) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw) ) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(direction);
        };

        /**
         *  Updates the camera.
         */
        void UpdateCamera()
        {
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        };

        /**
         *  Gets this Camera's current view (usually called by the Graphics for transforming objects).
         *  @return The view matrix associated with this Camera.
         */
        glm::mat4 GetView()
        {
            return view;
        };

        /** Gets this Camera's last mouse position in the x direction. */
        float GetX() { return lastPosX; };

        /** Gets this Camera's last mouse position in the y direction. */
        float GetY() { return lastPosY; };

        /** Gets this Camera's current mouse velocity. */
        float GetMouseVelocity() { return (velocity < 30.0f) ? velocity : 30.0f; };

    private:
        glm::vec3 cameraPos;    // Camera position data.
        glm::vec3 cameraFront;  // Camera front position.
        glm::vec3 cameraUp;     // Camera up position.
        glm::mat4 view;         // The current view matrix.
        float yaw;              // The current yaw value of the camera.
        float pitch;            // The current pitch value of the camera.
        float lastPosX;         // The last X position of the mouse.
        float lastPosY;         // The last Y position of the mouse.
        float velocity;         // The derived velocity of the mouse.

        GLFWwindow* window;
};

#endif