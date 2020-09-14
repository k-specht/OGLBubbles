#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "HelperStructs.h"

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
     *  @param color - The four-channel color vector to clear the buffer with. Channel 4 is alpha.
     */
    void ClearBuffer(Color color) noexcept;

    void DrawTriangle();

    /**
     *  Processes any inputs given to the window this Graphics instance is attached to.
     */
    void ProcessInput();

    /**
     *  Closes any remaining graphics resources attached to the active window.
     */
    void Close();
private:
    // A pointer to the window this Graphics instance paints to
    GLFWwindow* window;
};