#pragma once

#include "../cglph.h"
#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer(const void* data, uint32_t size);
	VertexBuffer(uint32_t size);
	~VertexBuffer();
	void Bind()	  { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }
	void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
	void SubmitData(const void* data, uint32_t size);

private:
	uint32_t m_RendererID;
};

class IndexBuffer
{
public:
	IndexBuffer(const void* data, uint32_t size);
	~IndexBuffer();
	void Bind()   { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }
	void Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

private:
	uint32_t m_RendererID;
};