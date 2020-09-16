#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Graphics
{
public:
    /**
     *  Initializes a Graphics object for the given window which will handle OpenGL drawing.
     *  @param window - A pointer to the GLFW window you wish to attach Graphics to.
     */
    Graphics(GLFWwindow* window);

    /**
     *  A basic copy operation on the Graphics object.
     *  Copying an object doesn't make sense, so we just delete it.
     */
    Graphics(const Graphics&) = delete;

    /**
     *  A basic move operation on the Graphics object.
     *  Moving an object doesn't make much sense, so we just delete it.
     */
    Graphics& operator=(const Graphics&) = delete;

    /**
     *  Deconstructor for the Graphics object.
     */
    ~Graphics();

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
    void ClearBuffer(
        float red,
        float blue,
        float green,
        float alpha
    );

    /**
     *  Draws a triangle to the screen using the graphics pipeline.
     */
    void DrawTriangle();

    /**
     *  Processes any inputs given to the window this Graphics instance is attached to.
     */
    void ProcessInput();

    /**
     *  Sets the maximum shader capacity.
     */
    void SetMaxSize(int size);

    /**
     *  Closes any remaining graphics resources attached to the active window.
     */
    void Close();
private:
    // A pointer to the window this Graphics instance paints to
    GLFWwindow* window;

    // The maximum shader capacity of this graphics object (defaults to 6)
    int maxSize;

    // Pointer to this Graphics object's shader program array
    unsigned int* shaders;
};