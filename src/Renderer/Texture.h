#pragma once

#include "../cglph.h"
#include <stb_image.h>

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();
	void Bind(uint32_t slot = 0);
	void Unbind();
	inline uint32_t GetWidth()  { return m_Width; }
	inline uint32_t GetHeight() { return m_Height; }

private:
	uint32_t m_RendererID;
	std::string m_FilePath;
	uint32_t m_Width, m_Height, m_BitPerPixel;
};