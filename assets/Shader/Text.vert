#version 330 core

layout (location = 0) in vec4 vertex; 						// vec2 2D position + vec2 texCoord

out vec2 TexCoords;

uniform mat4 u_Projection;

void main()
{
	// position from vertex
	gl_Position = u_Projection * vec4(vertex.xy, 0.0, 1.0);
	
	// texture coordinates from vertex
	TexCoords = vertex.zw;
}