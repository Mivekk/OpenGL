#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
	unsigned int m_IndexCount;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const IndexBuffer& ib);

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetIndexCount() const { return m_IndexCount; };
};