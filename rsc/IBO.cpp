#include "IBO.h"
#include <GL/glew.h>

ElementBuffer::ElementBuffer(unsigned int size, const void* data)
{
	glGenBuffers(1,&m_RendrerID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendrerID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1,&m_RendrerID);
}
void ElementBuffer::BindIBO()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendrerID);
}
void ElementBuffer::UnbindIBO()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
unsigned int ElementBuffer::getId()
{
	return m_RendrerID;
}
