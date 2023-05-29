#include "Texture.h"
#include "../Core/Log.h"
#include <glad/glad.h>

Texture::Texture(uint32_t color)
{
    // texture generation
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

    // filter and fill strategy
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // single color 1x1 texture (by default is white)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
}

Texture::Texture(const std::string& filePath, uint32_t texturePerRow, uint32_t texturePerColumn)
	: m_FilePath(filePath), m_TexturePerRow(texturePerRow), m_TexturePerColumn(texturePerColumn)
{
    LOG_TRACE("Loading texture from {}", filePath);

    // texture generation
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

    // filter and fill strategy
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR_MIPMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // image loading in RAM
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), (int*)&m_Width, (int*)&m_Height, (int*)&m_BitPerPixel, 0);

    // extract the format
    GLenum format = GL_RGB;
    switch (m_BitPerPixel)
    {
        case 1: format = GL_RED;  break;
        case 3: format = GL_RGB;  break;
        case 4: format = GL_RGBA; break;
        default: break;
    }

    // texture generation
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        // free RAM
        stbi_image_free(data);
    } else {
        LOG_ERROR("[{}] Texture loading failed", filePath);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(uint32_t slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}


glm::vec4 Texture::GetSubTextureCoordinates(uint32_t row, uint32_t column)
{
    // check if size are not defined
    if (!m_TexturePerRow || !m_TexturePerColumn)
        return glm::vec4({0.0f, 0.0f, 1.0f, 1.0f});           // x1, y1, x1 + wsize, y1 + hsize

    // get the size of a sub texture
    float subTextureWidth  = m_Width / m_TexturePerRow;
    float subTextureHeight = m_Height / m_TexturePerColumn;

    // get x1, y1 (left-up vertex coordinates)
    float x1 = row * subTextureWidth / m_Width;
    float y1 = column * subTextureHeight / m_Height;

    return glm::vec4({x1, y1, x1 + subTextureWidth / m_Width, y1 + subTextureHeight / m_Height});
}

TextureArray::TextureArray(uint32_t width, uint32_t height, uint32_t layerCount)
    : m_NextLayer(0), m_Width(width), m_Height(height), m_LayerCount(layerCount)
{
    // generate the array
    glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, layerCount);

    // fill and filtering option
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

TextureArray::~TextureArray()
{
    glDeleteTextures(1, &m_RendererID);
}

void TextureArray::Bind(uint32_t slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);
}

void TextureArray::Unbind()
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

uint32_t TextureArray::SubmitTexture(uint32_t color)
{
    // generate the single color texture on CPU side
    uint32_t* data = new uint32_t[m_Width * m_Height];
    for (uint32_t i = 0; i < m_Width * m_Height; ++i)
        data[i] = color;
    
    // submit the texture to the array
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, m_NextLayer, m_Width, m_Height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    m_NextLayer++;

    delete[] data;
    return m_NextLayer - 1;
}

glm::vec3 TextureArray::SubmitTexture(const std::string& filePath)
{
    uint32_t width, height, bitPerPixel;

    // TODO: implements nextLayer check

    // image loading in RAM
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), (int*)&width, (int*)&height, (int*)&bitPerPixel, 0);    
    
    // extract the format
    GLenum format = GL_RGBA;
    switch (bitPerPixel)
    {
        case 1: format = GL_RED;  break;
        case 3: format = GL_RGB;  break;
        case 4: format = GL_RGBA; break;
        default: break;
    }

    // submit texture to the layer and compute the texture normalized size that must be used
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, m_NextLayer, width, height, 1, format, GL_UNSIGNED_BYTE, data);
    float normalizedWidth  = (float)width  / (float)m_Width;
    float normalizedHeight = (float)height / (float)m_Height;
    glm::vec3 retInfo = { normalizedWidth, normalizedHeight, m_NextLayer };
    m_NextLayer++;
    
    // free RAM
    stbi_image_free(data);

    return retInfo;
}