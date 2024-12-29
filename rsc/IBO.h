#pragma once

class ElementBuffer
{
private:
	
public :
	unsigned int m_RendrerID;
	ElementBuffer(unsigned int size, const void* data);
	~ElementBuffer();
	void BindIBO();
	void UnbindIBO();
	unsigned int getId();
	
};

