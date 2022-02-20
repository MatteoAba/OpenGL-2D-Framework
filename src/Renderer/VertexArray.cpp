#include "VertexArray.h"

VertexLayout::VertexLayout()
	: m_Stride(0)
{
}

VertexLayout::~VertexLayout()
{
}

void VertexLayout::AddVertexGroup(VertexInfo group)
{
	m_Vertex.push_back(group);
	m_Stride += group.count * VertexInfo::GetSize(group.type);
}

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);
}

void VertexArray::AddBuffer(VertexLayout layout)
{
	uint32_t stride = layout.GetStride();
	uint32_t index = 0;
	uint64_t offset = 0;

	Bind();

	std::vector<VertexInfo> vertex = layout.GetVertex();
	for (auto group : vertex) {
		glVertexAttribPointer(index, group.count, VertexInfo::GetType(group.type), VertexInfo::GetNormalized(group.normalized), stride, (void*)offset);
		glEnableVertexAttribArray(index);
		index++;
		offset += group.count * VertexInfo::GetSize(group.type);
	}
}
