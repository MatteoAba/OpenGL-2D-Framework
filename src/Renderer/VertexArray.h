#pragma once

#include "../cglph.h"
#include <glad/glad.h>

enum DataType
{
	CGL_UINT32,
	CGL_FLOAT,
	CGL_BYTE
};

struct VertexInfo
{
	uint32_t type;
	uint32_t count;
	bool normalized;

	// utility to get size
	static uint32_t GetSize(uint32_t type)
	{
		switch (type)
		{
			case CGL_UINT32: return 4;
			case CGL_FLOAT:  return 4;
			case CGL_BYTE:   return 1;
		}

		return 0;
	}

	// utility to get OpenGL type
	static uint32_t GetType(uint32_t type)
	{
		switch (type)
		{
			case CGL_UINT32: return GL_UNSIGNED_INT;
			case CGL_FLOAT:  return GL_FLOAT;
			case CGL_BYTE:   return GL_UNSIGNED_BYTE;
		}

		return 0;
	}

	// utility to get OpenGL bool
	static uint32_t GetNormalized(bool normalized)
	{
		switch (normalized)
		{
			case true: return GL_TRUE;
			case false:  return GL_FALSE;
		}

		return 0;
	}
};

class VertexLayout
{
public:
	VertexLayout();
	~VertexLayout();
	void AddVertexGroup(VertexInfo group);
	uint32_t GetStride() { return m_Stride; }
	const std::vector<VertexInfo>& GetVertex() { return m_Vertex; }

private:
	uint16_t m_Stride;
	std::vector<VertexInfo> m_Vertex;
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void Bind()   { glBindVertexArray(m_RendererID); }
	void Unbind() { glBindVertexArray(0); }
	void AddBuffer(VertexLayout layout);

private:
	uint32_t m_RendererID;
};