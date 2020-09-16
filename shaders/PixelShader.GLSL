#version 420 core
out vec4 PixelColor;

//uniform vec4 timeColor;

in vec3 vColor;

void main()
{
    PixelColor = vec4( vColor, 1.0f );
}