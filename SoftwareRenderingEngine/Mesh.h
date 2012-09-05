#pragma once
#include "Triangle.h"
#include "Buffer.h"
#include "Type.h"
namespace JssEngine
{
	class Mesh
	{
	private:
		struct SETFVF
		{
			int vertex;
			int normal;
			int tex;
			int vertexsize;
		};

		void CreateTriangle(float* pBuffer,SETFVF vfvf);
		void CreateQuad(float* pBuffer,SETFVF vfvf);
		void CreateStrip(float* pBuffer,SETFVF vfvf);

	public:
		Triangle*	m_pTriangleList;		//삼각형 각각의 리스트...
		bool*		m_pBackFaceCulling;		//컬링되는삼각형이냐 Triangleindex = Cullingindex True = CULLING
		int			m_TriangleNum;			//삼각형 갯수

		DWORD		m_FVF;					//FVF타입

		int			m_MaterialNum;			//현재 메쉬의 재질 넘버링
		PRIMITE_TYPE m_PrimitedType;		//그리는 타입,Triangle,Strip,Quad

		int			m_start;				//시작점(버퍼에서의 버텍스)
		int			m_number;				//몇개 그리냐(버텍스 갯수)
	public:
		Mesh(void * buffer, DWORD FVF, int size,PRIMITE_TYPE type,int start, int Number);
		~Mesh(void);

	};
}