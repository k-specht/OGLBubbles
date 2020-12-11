#ifndef GRAPHICS
#define GRAPHICS

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Shader.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"


/**
 *  A class for encapsulating GLFW calls and the graphics pipeline.
 */
class Graphics
{
    public:
        /**
         *  Initializes a Graphics object for the given window which will handle OpenGL drawing.
         *  TODO: Organize this class's functions in a more meaningful manner.
         *  TODO: Implement the display lists in the tutorial for Chapter 7: https://www.opengl.org.ru/docs/pg/0208.html
         *  @param window - A pointer to the GLFW window you wish to attach Graphics to.
         *  @param cam    - A pointer to the Camera associated with this Graphics object.
         */
        Graphics(GLFWwindow* window, Camera* cam, float radius);

        /**
         *  A basic copy operation on the Graphics object.
         *  Copying an object doesn't make sense, so we just delete the function.
         */
        Graphics(const Graphics&) = delete;

        /**
         *  A basic move operation on the Graphics object.
         *  Moving an object doesn't make much sense, so we just delete the function.
         */
        Graphics& operator=(const Graphics&) = delete;

        /**
         *  Deconstructor for the Graphics object.
         */
        ~Graphics() 
        { 
            Close();
        };

        /**
         *  Presents the current frame in the draw buffer by swapping the two buffers.
         */
        void EndFrame();

        /**
         *  Clears the draw buffer using the specified color.
         *  @param red   - The red channel of this color.
         *  @param green - The green channel of this color.
         *  @param blue  - The blue channel of this color.
         *  @param alpha - The alpha channel of this color.
         */
        void ClearBuffer
        (
            float red,
            float blue,
            float green,
            float alpha
        );

        /**
         *  Creates shader resources to use during rendering.
         */
        void CreateShaders();

        /**
         *  Activates the last shader entered into the shader list.
         *  @param shaderID - The index of the shader to set as active.
         */
        void UseShader(int shaderID);

        /**
         *  Creates matrix transformations.
         *  @param width  - The screen width.
         *  @param height - The screen height.
         *  @param shaderID - The index of the shader to use.
         */
        void Transform
        (
            float width,
            float height,
            int shaderID
        );

        /**
         *  Creates matrix transformations for the light source.
         *  @param width  - The screen width.
         *  @param height - The screen height.
         *  @param shaderID - The index of the shader to use.
         */
        void TransformLight
        (
            float width,
            float height,
            int shaderID
        );

        /**
         *  Generates bindables for a Triangle.
         *  @param index - The index of the VAO for this drawable object.
         */
        void GenerateTriangle(int index);

        /**
         *  Generates bindables for a Rectangle.
         *  @param index - The index of the VAO for this drawable object.
         */
        void GenerateRectangle(int index);

        /**
         *  Generates bindables for a Cube.
         *  @param index - The index of the VAO for this drawable object.
         */
        void GenerateCube(int index);

        /**
         *  Generates bindables for a Sphere.
         *  @param index - The index of the VAO for this drawable object.
         */
        void GenerateSphere(int index);

        /**
         *  Recalculates the vertex array object for the sphere.
         */
        void RegenSphere(int index);

        /**
         *  Generates bindables for a cluster of points using input.txt.
         *  @param index - The index of the VAO for this drawable object.
         */
        void GenerateCluster(int index);

        /**
         *  Generates bindables for a Centroid.
         *  @param index  - The index of the VAO for this drawable object.
         *  @param center - The xyz coordinates of the center of this circle.
         *  @param radius - The radius for this circle.
         */
        //void GenerateCentroid(int index, float center[3]), float radius;


        /**
         *  Draws a Centroid.
         *  @param index  - The index of the VAO for this drawable object.
         *  @param center - The xyz coordinates of the center of this circle.
         *  @param radius - The radius for this circle.
         */
        //void DrawCentroid(int index, float center[3], float radius);

        /**
         *  Draws a sphere to the screen using the graphics pipeline.
         *  @param index - The index of the VAO for this drawable object.
         */
        void DrawSphere(int index, int shaderID);

        /**
         *  Draws a cube to the screen using the graphics pipeline.
         *  @param index - The index of the VAO for this drawable object.
         */
        void DrawCube(int index, int shaderID);

        /**
         *  Draws a triangle to the screen using the graphics pipeline.
         *  @param index - The index of the VAO for this drawable object.
         */
        void DrawTriangle(int index, int shaderID);

        /**
         *  Draws a triangle to the screen using the graphics pipeline.
         *  @param index - The index of the VAO for this drawable object.
         */
        void DrawRectangle(int index, int shaderID);


        /**
         *  Uses the Centroid mini-project to draw a cluster and containing sphere.
         *  @param index - The index of the VAO for this drawable object.
         *  @param n     - The number of vertices to draw.
         */
        void DrawCluster(int index, int n);

        /**
         *  Processes any inputs given to the window this Graphics instance is attached to.
         */
        void ProcessInput();

        /**
         *  Sets the maximum shader capacity.
         *  @param size - The new maximum shader capacity for this Graphics instance.
         */
        void SetMaxSize(int size);

        /**
         *  Closes any remaining graphics resources attached to the active window.
         */
        void Close();

        /**
         *  Updates the camera position to whatever the resulting matrix is from processing inputs.
         */
        void UpdateCamera();

        /**
         *  A callback function for GLFW to manage mouse data.
         *  @param window - The GLFW window whose mouse data is being handled.
         *  @param xpos   - The x position of the mouse.
         *  @param ypos   - The y position of the mouse.
         */
        void Mouse
        (
            GLFWwindow* window, 
            double xpos, 
            double ypos
        );

        /**
         *  Checks for any collisions between the x/y positions of the mouse and the active sphere object.
         *  Chains the Collision function with an appropriate force vector if a collision is found.
         *  @param x        - The x position of the mouse.
         *  @param y        - The y position of the mouse.
         *  @param velocity - The velocity of the mouse.
         */
        void CollisionCheck(float x, float y, float velocity);

        /**
         *  Applies a collision of the specified magnitude to the active sphere in this Graphics instance.
         *  @param vertex    - The position of the impact, assumed to be a force direction toward the center point.
         *  @param magnitude - The magnitude of impact force to be applied to this collision event.
         */
        void Collision(std::array<float,3> vertex, float magnitude);

    private:
        
        GLFWwindow* window; // A pointer to the window this Graphics instance paints to.

        int maxSize;        // The maximum shader capacity of this graphics object (defaults to 6).

        glm::vec3 lightPos = {1.2f, 1.0f, 2.0f}; // The position of the light source in this graphics scene.

        std::vector<Shader*> shaders; // This Graphics object's shader vector; holds shader programs for OpenGL.

        unsigned int* VAOs; // Pointer to this Graphics object's Vertex  Array  Object array.
        unsigned int* VBOs; // Pointer to this Graphics object's Vertex  Buffer Object array.
        unsigned int* EBOs; // Pointer to this Graphics object's Element Buffer Object array.
        
        Sphere* sphere;     // Pointer to this Graphics object's sphere object (TODO: Refactor code so this isn't used).
        Camera* camera;     // The camera associated with this Graphics object
};

#endif