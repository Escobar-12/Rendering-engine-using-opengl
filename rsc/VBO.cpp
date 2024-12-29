#include "VBO.h"
#include <GL/glew.h>

VertexBuffer::VertexBuffer(unsigned int size, const void* data)
{
	glGenBuffers(1, &m_RendrerID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendrerID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendrerID);
}
void VertexBuffer::BindVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendrerID);
}
void VertexBuffer::UnBindVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
unsigned int VertexBuffer::getId()
{
	return m_RendrerID;
}