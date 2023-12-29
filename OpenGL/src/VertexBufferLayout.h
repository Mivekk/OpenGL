#pragma once

#include <GL/glew.h>

#include <vector>
#include <assert.h>

struct VertexBufferLayoutElement
{
	unsigned int type;
	unsigned int count;
	unsigned int offset;
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferLayoutElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count);

	inline const std::vector<VertexBufferLayoutElement>& GetElements() const { return m_Elements; };
	inline unsigned int GetStride() const { return m_Stride; }
};

template<typename T>
inline void VertexBufferLayout::Push(unsigned int count)
{
	assert(false);
}

template<>
inline void VertexBufferLayout::Push<float>(unsigned int count)
{
	m_Elements.push_back({ GL_FLOAT, count, m_Stride });
	m_Stride += count * sizeof(float);
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
	m_Elements.push_back({ GL_UNSIGNED_INT, count, m_Stride });
	m_Stride += count * sizeof(unsigned int);
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
	m_Elements.push_back({ GL_UNSIGNED_BYTE, count, m_Stride });
	m_Stride += count * sizeof(unsigned char);
}