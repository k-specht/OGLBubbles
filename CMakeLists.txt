cmake_minimum_required(VERSION 3.17)
message("Creating build information for OGLBubbles...")

# Creates Visual Studio SLN & vcpkg files for project
project(OGLBubbles VERSION 1.0.0 DESCRIPTION "An OpenGL project that renders soap bubble physics.")

# Include CMake packages for DLL's & package management
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include (GenerateExportHeader)
include(CTest)

# Sets up package
CONFIGURE_PACKAGE_CONFIG_FILE(${CMAKE_CURRENT_SOURCE_DIR}/src/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/OGLBubblesConfig.cmake"
    INSTALL_DESTINATION lib/cmake
)
WRITE_BASIC_PACKAGE_VERSION_FILE(OGLBubblesConfigVersion.cmake
    #[VERSION <1.0.0>]
    COMPATIBILITY AnyNewerVersion
)

# Sets up CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Configures linux-style package by expanding the @ macros in CSolver.pc.in
configure_file(src/OGLBubbles.pc.in OGLBubbles.pc @ONLY)
configure_file(src/OGLBubblesConfig.h.in src/OGLBubblesConfig.h)

# Source binaries to be compiled
set(SOURCES
    src/OGLBubbles.cpp
    src/Graphics.cpp
    src/Shader.cpp
    src/Sphere.cpp
    src/OGLBLOG.cpp
)

# Header files for binaries
set(HEADERS
    src/OGLBubbles.hpp
    src/Graphics.hpp
    src/Shader.hpp
    src/Sphere.hpp
    src/OGLBLOG.hpp
    src/OGLBubblesConfig.h
)

#Creates executable
add_executable(OGLBubbles
              ${SOURCES}
              ${HEADERS}
)

# Adds include directories to compiler command
target_include_directories(OGLBubbles PRIVATE
                          $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
                          $<INSTALL_INTERFACE:include>
                          ${PROJECT_BINARY_DIR}
)

# Adds link directories to linker
target_link_directories(OGLBubbles PRIVATE
                      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>  
                      $<INSTALL_INTERFACE:lib>
)

# Unused
#find_package(OpenGL REQUIRED)
#target_link_libraries(OGLBubbles OpenGL::GL)
#add_library(lib/GLFW glfw3dll.lib)
#add_subdirectory(lib GLFW)
#target_link_libraries(OGLBubbles PUBLIC lib)

# Sets properties
set_target_properties(OGLBubbles PROPERTIES VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Adds installation logic
install(TARGETS OGLBubbles
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        PUBLIC_HEADER DESTINATION include
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/OGLBubbles.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/OGLBubblesConfig.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/OGLBubblesConfigVersion.cmake"
        DESTINATION lib/cmake
)
