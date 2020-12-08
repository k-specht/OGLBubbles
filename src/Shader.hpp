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

        ~Shader()  { }; // Deconstructor
        
        /**
         *  Activates this shader program.
         *  Every rendering call from this function onward will use this Shader object's shader program.
         */
        void use();
        
        // TODO: Delete these bc they're unused
        //void setBool(const std::string &name, bool value) const;  
        //void setInt(const std::string &name, int value) const;   
        //void setFloat(const std::string &name, float value) const;
};
  
#endif