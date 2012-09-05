#include "Mesh.h"
#include <string.h>

namespace JssEngine
{

	Mesh::Mesh(void * buffer, DWORD FVF, int size,PRIMITE_TYPE type,int start, int Number)
	{
		m_MaterialNum	= -1;
		m_FVF			= FVF;
		m_PrimitedType	= type;
		m_start			= start;
		m_number		= Number;

		float *pCalc;
		pCalc = (float*)buffer;

		SETFVF vfvf;
		vfvf.vertex		= 0;
		vfvf.normal		= 0;
		vfvf.tex		= 0;
		vfvf.vertexsize = 0;

		if( (m_FVF & JSSFVF_XYZ) == JSSFVF_XYZ)
			vfvf.vertex = 3;
		if((m_FVF & JSSFVF_NORMAL) == JSSFVF_NORMAL)
			vfvf.normal = 3;
		if((m_FVF & JSSFVF_TEX0) == JSSFVF_TEX0)
			vfvf.tex = 2;

		vfvf.vertexsize = vfvf.vertex + vfvf.tex + vfvf.normal;

	
		switch(type)
		{
		case JSS_PRIMITE_TRIANGLE:
			m_TriangleNum = (Number)/3;
			break;
		case JSS_PRIMITE_QUAD:
			m_TriangleNum = (Number)/2;
			break;
		case JSS_PRIMITE_STRIP:
			m_TriangleNum = (Number)-2;
			break;
		}
		m_pTriangleList		= new Triangle[m_TriangleNum];
		m_pBackFaceCulling	= new bool[m_TriangleNum];

		//초기값 다 False로 컬링...
		memset(m_pBackFaceCulling,0,m_TriangleNum);

		//Trinagle 만 지원됨 일단...(index버퍼도 생각을 해야됨 -_-;;)
		switch(type)
		{
		case JSS_PRIMITE_TRIANGLE:
			CreateTriangle(pCalc,vfvf);
			break;
		case JSS_PRIMITE_QUAD:
			CreateQuad(pCalc,vfvf);
			break;
		case JSS_PRIMITE_STRIP:
			CreateStrip(pCalc,vfvf);
			break;
		}

	}
	Mesh::~Mesh(void)
	{
		if(m_pTriangleList)
			delete[] m_pTriangleList;
		if(m_pBackFaceCulling)
			delete[] m_pBackFaceCulling;
	}


	void Mesh::CreateTriangle(float* pBuffer,SETFVF vfvf)
	{
		//삼각형 단위 하나당
		Vector3f v;
		Vector3f n;
		Vector2f uv;

		int normalnum = vfvf.vertex;
		int texnum = vfvf.vertex+vfvf.normal;
		int vertexnum = (vfvf.vertexsize)*m_start;

		for(int i = 0; i < m_TriangleNum; i++)
		{
			for(int j=0; j< 3; j++)
			{
				if(vfvf.vertex == 3)
				{
					m_pTriangleList[i].m_vt[j].m_XYZ._x = pBuffer[vertexnum++];
					m_pTriangleList[i].m_vt[j].m_XYZ._y = pBuffer[vertexnum++];
					m_pTriangleList[i].m_vt[j].m_XYZ._z = pBuffer[vertexnum++];
					vertexnum +=(vfvf.normal+ vfvf.tex);
				}
				if(vfvf.normal == 3)
				{
					m_pTriangleList[i].m_vt[j].m_Normal._x = pBuffer[normalnum++];
					m_pTriangleList[i].m_vt[j].m_Normal._y = pBuffer[normalnum++];
					m_pTriangleList[i].m_vt[j].m_Normal._z = pBuffer[normalnum++];
					normalnum += (vfvf.vertex+vfvf.tex);
				}
				if(vfvf.tex == 2)
				{	
					m_pTriangleList[i].m_vt[j].m_UV1._x = pBuffer[texnum++];
					m_pTriangleList[i].m_vt[j].m_UV1._y = pBuffer[texnum++];
					texnum += (vfvf.vertex+vfvf.normal);
				}
			}
		}
	}
	void Mesh::CreateQuad(float* pBuffer,SETFVF vfvf)
	{

	}
	void Mesh::CreateStrip(float* pBuffer,SETFVF vfvf)
	{

	}


}