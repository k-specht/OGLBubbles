cmake_minimum_required(VERSION 3.17)
project(OGLBubbles)

configure_file(src/OGLBConfig.h.in src/OGLBConfig.h)





set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include_directories(include)



add_executable(OGLBubbles
  src/OGLBubbles.cpp
  include/glad/glad
  include/GLFW/glfw3
)

#add_library(lib/GLFW glfw3dll.lib)
#add_subdirectory(lib GLFW)
#target_link_libraries(OGLBubbles PUBLIC lib)

target_include_directories(OGLBubbles PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

#set(CMAKE_CXX_STANDARD 17)