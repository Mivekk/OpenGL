#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray()
	: m_IndexCount(0)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const IndexBuffer& ib)
{
	Bind();
	vb.Bind();
	ib.Bind();

	m_IndexCount = ib.GetCount();

	const auto& elements = layout.GetElements();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, GL_FALSE, layout.GetStride(), (const void*)element.offset));
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
