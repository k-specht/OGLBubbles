#include "Shader.h"
#include "BubbleError.h"
/**
 *  Shader.cpp
 *  The file reading method was partially adapted from the LearnOpenGL's Shader tutorial.
 */


Shader::Shader(const char* vertexPath, const char* pixelPath)
{
    // Retrieves the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string pixelCode;
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
        pixelCode = pShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        throw BubbleError(e.what(),0,0);
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

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
        throw BubbleError(infoLog, 512, 0);
    }

    // Shader generation (pixel)
    unsigned int pixelShader;
    pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(pixelShader, 1, &pShaderCode, NULL);
    glCompileShader(pixelShader);

    glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(pixelShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PIXEL::COMPILATION_FAILED" << std::endl;
        throw BubbleError(infoLog, 512, 0);
    }

    // Creates shader program from the two compiled shaders
    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, pixelShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if(!success) 
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED" << std::endl;
        throw BubbleError(infoLog, 512, 0);
    }

    // Removes unneeded data and sets the Shader object's id
    Shader::use();
    glDeleteShader(vertexShader);
    glDeleteShader(pixelShader);
}

void Shader::use()
{
    glUseProgram(Shader::ID);
}