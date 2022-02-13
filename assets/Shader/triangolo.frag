#version 440 core

in vec3 colors;
out vec4 FragColor;

void main()
{    
    FragColor = vec4(colors, 1.0);
}