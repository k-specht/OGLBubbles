#ifndef SHADER_H
#define SHADER_H  

class Shader
{
    public:
        // the program ID
        unsigned int ID;
    
        /**
         *  Reads and compiles the shaders at the specified filename.
         *  @param vertexShaderPath - The path and filename of the vertex shader you want to load.
         *  @param pixelShaderPath  - The path and filename of the pixel shader you want to load.
         */
        Shader(const char* vertexShaderPath, const char* pixelShaderPath);

        /**
         *  Reads and compiles the shaders at the specified filename.
         *  @param vertexShaderPath - The path and filename of the vertex shader you want to load.
         *  @param pixelShaderPath  - The path and filename of the pixel shader you want to load.
         *  @param geoPath          - The path and filename of the geometry shader you want to load.
         */
        Shader(const char* vertexShaderPath, const char* pixelShaderPath, const char* geoPath);

        ~Shader()  { }; // Deconstructor
        
        /**
         *  Activates this shader program.
         *  Every rendering call from this function onward will use this Shader object's shader program.
         */
        void use();
};
  
#endif