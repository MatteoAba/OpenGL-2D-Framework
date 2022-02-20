#version 440 core

in vec3 colors;
in vec2 texCoords;

uniform sampler2D texture1;

out vec4 FragColor;

void main()
{    
    FragColor = mix(texture(texture1, texCoords), vec4(colors, 1.0), 0.2);
}