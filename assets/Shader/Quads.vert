#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 colors;
out vec2 texCoords;

void main()
{	
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
	colors = aColor;
	texCoords = aTexCoords;
}