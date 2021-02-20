#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 PixPos;
out vec3 lightPos;

//out vec3 vColor;
//uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    PixPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // TODO: Move this to CPU
    lightPos = light;
    //Normal = vec3( aNormal );
    //vColor = aColor;
}