#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTextureCoords;
layout (location = 3) in float aTextureSlotID;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 colors;
out vec2 textureCoords;
out float textureSlotID;

void main()
{	
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
	colors = aColor;
	textureCoords = aTextureCoords;
    textureSlotID = aTextureSlotID;
}