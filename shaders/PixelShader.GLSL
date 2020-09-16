#version 420 core
out vec4 PixelColor;

uniform vec4 timeColor;

void main()
{
    PixelColor = vec4(1.0f, timeColor.y, 0.2f, 1.0f);
}