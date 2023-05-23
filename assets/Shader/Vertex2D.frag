#version 440 core

in vec4 colors;
in vec2 textureCoords;
in float textureSlotID;

uniform sampler2D u_Textures[32];

out vec4 FragColor;

void main()
{    
    int index = int(textureSlotID);
    FragColor = texture(u_Textures[index], textureCoords) * colors;
}