#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

            float yaw = 0.0f;
            float pitch = 0.0f;
            float lastPosX = 0.0f;
            float lastPosY = 0.0f;
        }
        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;
        ~Camera();

        /**
         *  Processes the movement of the mouse.
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

            float xOff = xPos - lastPosX;
            float yOff = lastPosY - yPos; // reversed since y-coordinates go from bottom to top

            lastPosX = xPos;
            lastPosY = yPos;

            float sensitivity = 0.1f;
            xOff *= sensitivity;
            yOff *= sensitivity;

            yaw   += xOff;
            pitch += yOff;

            if(pitch > 89.0f)
                pitch = 89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;

            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(direction);
        }

        void ProcessInput()
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

        }

        /**
         *  Updates the camera.
         */
        void UpdateCamera()
        {
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        }

        /**
         *  Gets this Camera's current view (usually called by the Graphics for transforming objects).
         *  @return The view matrix associated with this Camera.
         */
        glm::mat4 GetView()
        {
            return view;
        }

    private:
    // Camera position data
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::mat4 view;
    float yaw;
    float pitch;
    float lastPosX;
    float lastPosY;

    GLFWwindow* window;
};

#endif