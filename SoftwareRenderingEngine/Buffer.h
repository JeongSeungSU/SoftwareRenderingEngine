#pragma once
#include "Mathematics.h"

namespace JssEngine
{
	class VertexBuffer
	{
	private:
		void				*m_pBuffer;
		unsigned int		m_BufferSize;
		unsigned int		m_Size;

		DWORD				m_FVF;
		
	public:
		VertexBuffer(void);
		~VertexBuffer(void);
	public:
		void InitBuffer(int Length,DWORD FVF);
		void Lock(unsigned int start, unsigned int Size,void ** pDate);
		void Unlock();
		int	 GetSize() {return m_Size;}
		int  GetBufferSize() {return m_BufferSize;}
		DWORD GetFVF() {return m_FVF;}
		void ReleaseBuffer();
		void imsiprint();
	};

}