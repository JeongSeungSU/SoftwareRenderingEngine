#include "Buffer.h"
#include <iostream>


namespace JssEngine
{
	VertexBuffer::VertexBuffer(void)
	:m_pBuffer(0),m_Size(0),m_FVF(0)
	{
	}

	VertexBuffer::~VertexBuffer(void)
	{

	}

	void VertexBuffer::Lock(unsigned int start, unsigned int Size,void ** pDate)
	{
		//비디오 메모리에 넣고 쓰는거라 락이 필요하지만 여기선 필요 없네-_-?쓰레드 쓰는것도 아니고 쓸때는 필요하긴 할려나 -_-;
		if(start > m_BufferSize-1)
		{
			*pDate = 0;
			return;
		}

		BYTE* pbf = reinterpret_cast<BYTE*> (m_pBuffer);

		 *pDate = (void*)&pbf[start];

	}
	void VertexBuffer::Unlock()
	{

	}
	void VertexBuffer::InitBuffer(int Length,DWORD FVF)
	{
		m_BufferSize = Length;
		m_FVF  = FVF;
		m_Size = m_BufferSize/sizeof(float);

		m_pBuffer = new char[m_BufferSize];
	}

	void VertexBuffer::imsiprint()
	{
// 		float * p = (float*)m_pBuffer;
// 
// 		for(int i =0; i< m_Size/5; i++)
// 		{
// 			std::cout<<"Vertex = "<<i<<"   X : "<<p[(i*5)]<<"   Y : "<<p[(i*5)+1]<<"   Z : "<<p[(i*5)+2]<<std::endl;
// 		}
	}
	void VertexBuffer::ReleaseBuffer()
	{
		if(m_pBuffer)
		{
			delete m_pBuffer;
			m_pBuffer = 0;
		}
		delete this;
	}

}