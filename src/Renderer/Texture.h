#pragma once

#include "../oglph.h"
#include <stb_image.h>

class Texture
{
public:
	Texture(uint32_t color = 0xffffffff);
	Texture(const std::string& filePath, uint32_t texturePerRow = 0, uint32_t texturePerColumn = 0);
	~Texture();
	void Bind(uint32_t slot = 0);
	void Unbind();
	inline uint32_t GetWidth()  { return m_Width; }
	inline uint32_t GetHeight() { return m_Height; }
	glm::vec4 GetSubTextureCoordinates(uint32_t row, uint32_t column);

private:
	uint32_t m_RendererID;
	std::string m_FilePath;
	uint32_t m_Width, m_Height, m_BitPerPixel;
	uint32_t m_TexturePerRow, m_TexturePerColumn; 
};

class TextureArray
{
public:
	TextureArray(uint32_t width, uint32_t height, uint32_t layerCount);
	~TextureArray();
	void Bind(uint32_t slot = 0);
	void Unbind();
	uint32_t  SubmitTexture(uint32_t color = 0xFFFFFFFF);
	glm::vec3 SubmitTexture(const std::string& filePath);

private:
	uint32_t m_RendererID;
	uint32_t m_NextLayer;
	uint32_t m_Width, m_Height, m_LayerCount;
};