#pragma once

#include "../Core/Application.h"
#include "../Renderer/Renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

// character structure
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class Text
{
public:
	Text(const std::string& fontPath, Application* owner);
	~Text();

	void RenderText(std::string text, glm::vec2 position, float scale, glm::vec3 color, Framebuffer* FBO = nullptr);

private:
	Application* m_Owner;

	// characters map
	std::map<GLchar, Character> m_Characters;

	// buffers and shader 
	VertexArray*  m_VAO;
	VertexBuffer* m_VBO;
	IndexBuffer*  m_IBO;
	Shader* m_Shader;
	glm::mat4 m_Projection;
};