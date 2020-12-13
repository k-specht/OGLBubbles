#include "Shader.hpp"

#include <glad/glad.h>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//#include "OGLBLOG.hpp" // Linker doesn't like this :<

/**
 *  Shader.cpp
 *  TODO: Refactor error handling.
 *  The file reading method was partially adapted from the LearnOpenGL's Shader tutorial.
 */

Shader::Shader(const char* vertexPath, const char* pixelPath)
{
    // Retrieves the vertex/fragment source code from filePath
    std::string   vertexCode;
    std::string   pixelCode;
    std::ifstream vShaderFile;
    std::ifstream pShaderFile;

    // Ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    pShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // Opens files
        vShaderFile.open(vertexPath);
        pShaderFile.open(pixelPath);

        // Reads file's buffer contents into streams
        std::stringstream vShaderStream, pShaderStream;
        
        vShaderStream << vShaderFile.rdbuf();
        pShaderStream << pShaderFile.rdbuf();

        // Closes file handlers
        vShaderFile.close();
        pShaderFile.close();

        // Converts streams into strings
        vertexCode   = vShaderStream.str();
        pixelCode    = pShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        //Log::e(e.what()); // Linker hates this for some reason??
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* pShaderCode = pixelCode.c_str();

    // Shader generation (vertex)
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if ( !success )
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        //Log::e(infoLog); // Linker hates this for some reason??
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    // Shader generation (pixel)
    unsigned int pixelShader;
    pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(pixelShader, 1, &pShaderCode, NULL);
    glCompileShader(pixelShader);

    glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &success);

    if ( !success )
    {
        glGetShaderInfoLog(pixelShader, 512, NULL, infoLog);
        //Log::e(infoLog); // Linker hates this for some reason??
        std::cerr << "ERROR::SHADER::PIXEL::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    // Creates shader program from the two compiled shaders
    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, pixelShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if ( !success ) 
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        //Log::e(infoLog); // Linker hates this for some reason??
        std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    // Removes unneeded data and sets the Shader object's id
    this->use();
    glDeleteShader(vertexShader);
    glDeleteShader(pixelShader);
    //delete vShaderCode;
    //delete pShaderCode;
}

Shader::Shader(const char* vertexPath, const char* pixelPath, const char* geoPath)
{
    // Retrieves the vertex/fragment source code from filePath
    std::string   vertexCode;
    std::string   pixelCode;
    std::string   geoCode;

    std::ifstream vShaderFile;
    std::ifstream pShaderFile;
    std::ifstream geoFile;

    // Ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    pShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    geoFile.exceptions     (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // Opens files
        vShaderFile.open(vertexPath);
        pShaderFile.open(pixelPath);
        geoFile.open(geoPath);

        // Reads file's buffer contents into streams
        std::stringstream vShaderStream, pShaderStream, geoStream;
        
        vShaderStream << vShaderFile.rdbuf();
        pShaderStream << pShaderFile.rdbuf();
        geoStream     << geoFile.rdbuf();

        // Closes file handlers
        vShaderFile.close();
        pShaderFile.close();
        geoFile.close();

        // Converts streams into strings
        vertexCode   = vShaderStream.str();
        pixelCode    = pShaderStream.str();
        geoCode      = geoStream.str();
    }
    catch(std::ifstream::failure e)
    {
        //Log::e(e.what()); // Linker hates this for some reason??
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
        return;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* pShaderCode = pixelCode.c_str();
    const char* geoShaderCode = geoCode.c_str();

    // Shader generation (vertex)
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if ( !success )
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        //Log::e(infoLog); // Linker hates this for some reason??
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    // Shader generation (pixel)
    unsigned int pixelShader;
    pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(pixelShader, 1, &pShaderCode, NULL);
    glCompileShader(pixelShader);

    glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &success);

    if ( !success )
    {
        glGetShaderInfoLog(pixelShader, 512, NULL, infoLog);
        //Log::e(infoLog); // Linker hates this for some reason??
        std::cerr << "ERROR::SHADER::PIXEL::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    // Shader generation (geometry)
    unsigned int geoShader;
    geoShader = glCreateShader(GL_GEOMETRY_SHADER);

    glShaderSource(geoShader, 1, &geoShaderCode, NULL);
    glCompileShader(geoShader);

    glGetShaderiv(geoShader, GL_COMPILE_STATUS, &success);

    if ( !success )
    {
        glGetShaderInfoLog(geoShader, 512, NULL, infoLog);
        //Log::e(infoLog); // Linker hates this for some reason??
        std::cerr << "ERROR::SHADER::PIXEL::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    // Creates shader program from the two compiled shaders
    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, pixelShader);
    glAttachShader(ID, geoShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if ( !success ) 
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        //Log::e(infoLog); // Linker hates this for some reason??
        std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    // Removes unneeded data and sets the Shader object's id
    this->use();
    glDeleteShader(vertexShader);
    glDeleteShader(pixelShader);
    glDeleteShader(geoShader);
    //delete vShaderCode;
    //delete pShaderCode;
    //delete geoCode;
}

void Shader::use()
{
    glUseProgram(this->ID);
}