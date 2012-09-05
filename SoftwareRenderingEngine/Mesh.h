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
		Triangle*	m_pTriangleList;		//�ﰢ�� ������ ����Ʈ...
		bool*		m_pBackFaceCulling;		//�ø��Ǵ»ﰢ���̳� Triangleindex = Cullingindex True = CULLING
		int			m_TriangleNum;			//�ﰢ�� ����

		DWORD		m_FVF;					//FVFŸ��

		int			m_MaterialNum;			//���� �޽��� ���� �ѹ���
		PRIMITE_TYPE m_PrimitedType;		//�׸��� Ÿ��,Triangle,Strip,Quad

		int			m_start;				//������(���ۿ����� ���ؽ�)
		int			m_number;				//� �׸���(���ؽ� ����)
	public:
		Mesh(void * buffer, DWORD FVF, int size,PRIMITE_TYPE type,int start, int Number);
		~Mesh(void);

	};
}