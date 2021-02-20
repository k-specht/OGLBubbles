#version 420 core
out vec4 PixelColor;

in vec3 Normal;
in vec3 PixPos;
in vec3 lightPos;

//uniform vec4 timeColor;
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 viewPos;

//in vec3 vColor;

/*vec3 normalize(vec3 n)
{
    float length = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);

    n[0] /= length;
    n[1] /= length;
    n[2] /= length;

    return n;
}*/

void main()
{
    // Ambient lighting
    vec3  ambient    = (0.1f) * lightColor;
    vec3  dist       = lightPos - PixPos;
    float intensity  = 1 / sqrt(dist[0] * dist[0] + dist[1] * dist[1] + dist[2] * dist[2]);

    // Diffuse lighting
    vec3  norm       = normalize(Normal);
    vec3  lightDir   = normalize(lightPos - PixPos);
    
    vec3  diff       = max(dot(norm, lightDir), 0.0) * intensity * lightColor;

    // Specular lighting
    float specStr    = 0.5f;
    vec3  viewDir    = normalize(viewPos - PixPos);
    vec3  reflectDir = reflect(-lightDir, norm);

    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), 4);
    vec3  specular   = specStr * spec * intensity * lightColor;

    // Lighting calculation
    //vec3  result     = (ambient + diff) * objectColor;
    vec3  result     = (ambient + diff + specular) * objectColor;
    PixelColor       = vec4(result, 1.0f);
}