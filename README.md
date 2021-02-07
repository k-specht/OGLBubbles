# OGLBubbles
An OpenGL project that aims to create soap bubble lookalikes that move in a realistic manner.

Installation:
------------------------------------------------------------------------------------------------------------
1. Clone this repository
2. Run the following CMake commands from the project directory

mkdir build

cd build

cmake ../

cd ..

cmake --build ./build --config Release

cmake --install ./build --prefix "/install/path"

Running the Project:
------------------------------------------------------------------------------------------------------------

Make sure that glfw3.dll is in the same directory as the executable, and the shaders folder is in the
project directory.

Start the executable file and use WASD to move the camera in world space, IJKL to move the light source,
and the arrow keys to move the camera's viewport.

Notes:
------------------------------------------------------------------------------------------------------------
Check the examples directory for details on the output of this program.

This project is mostly a way for me to learn OpenGL software development, but the final result might be an 
interesting effect to implement for small games.

It's also an example of how to use CMake to build and install a clean executable!
