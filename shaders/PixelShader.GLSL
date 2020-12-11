#version 420 core
out vec4 PixelColor;

in vec3 Normal;
in vec3 PixPos;

//uniform vec4 timeColor;
uniform vec3 objectColor;
uniform vec3 lightColor;

// Since light position is static, this is fine
uniform vec3 lightPos; 

//in vec3 vColor;

void main()
{
    // Ambient lighting
    float ambient  = (0.1) * lightColor;
    

    // Diffuse lighting
    vec3  norm     = normalize(Normal);
    vec3  lightDir = normalize(lightPos - PixPos);
    
    vec3  diffuse  = max(dot(norm, lightDir), 0.0) * lightColor;

    // Lighting calculation
    vec3  result   = (ambient + diff) + diff * objectColor;
    PixelColor     = vec4(result, 1.0);

    // Old static light version
    //PixelColor   = vec4( lightColor * objectColor, 1.0f );
}