#pragma once

class VertexBuffer
{
	private :
		
	public:
		unsigned int m_RendrerID;
		VertexBuffer(unsigned int size, const void* data);
		~VertexBuffer();
		void BindVBO();
		void UnBindVBO();
		unsigned int getId();

};

