#include "CullingModule.h"

namespace JssEngine
{

	CullingModule::CullingModule(void)

	{
	}

	CullingModule::~CullingModule(void)
	{
	}
	void CullingModule::BackFaceCulling(Mesh* pMesh, Vector3f CameraDir, DWORD cullmode)
	{
		Vector3f v1,v2,normal;
		float innervalue;

		if(cullmode == JSS_CULL_NONE)
			return;
		//노말 계산
		for(int i =0; i< pMesh->m_TriangleNum; i++)
		{
			v1 = (pMesh->m_pTriangleList[i].m_vt[1].m_XYZ - pMesh->m_pTriangleList[i].m_vt[0].m_XYZ);
			v2 = (pMesh->m_pTriangleList[i].m_vt[2].m_XYZ - pMesh->m_pTriangleList[i].m_vt[1].m_XYZ);

			normal = VectorCross(v1,v2);
			Nomalize(normal);

			innervalue = VectorInnerProduct(CameraDir,normal);

			pMesh->m_pTriangleList[i].m_TriangleNormal = normal;


			if(cullmode == JSS_CULL_CCW)
			{
				//90도 정확히 90도면 버림...
				if(innervalue > 0)
					pMesh->m_pBackFaceCulling[i] = false;
				else
					pMesh->m_pBackFaceCulling[i] = true;
			}
			else if(cullmode == JSS_CULL_CW)
			{
				if(innervalue > 0)
					pMesh->m_pBackFaceCulling[i] = true;
				else
					pMesh->m_pBackFaceCulling[i] = false;
			}


		}
	}
}