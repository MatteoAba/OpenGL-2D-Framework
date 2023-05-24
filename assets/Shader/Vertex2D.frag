#version 440 core

in vec4 colors;
in vec2 textureCoords;
in float textureSlotID;

uniform sampler2DArray u_Textures;

out vec4 FragColor;

void main()
{    
    int index = int(textureSlotID);
    FragColor = texture(u_Textures, vec3(textureCoords, textureSlotID)) * colors;
}