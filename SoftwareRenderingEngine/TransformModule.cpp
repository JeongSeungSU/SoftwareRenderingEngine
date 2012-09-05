#include "TransformModule.h"
#include <string.h>
#include <math.h>

namespace JssEngine
{
	TransformModule::TransformModule(void)
	{
	}

	TransformModule::~TransformModule(void)
	{
	}

	void TransformModule::TransformView(Vector3f Up, Vector3f dir, Vector3f right, Vector3f position, Matrix16f& viewmat)
	{
		memset(&viewmat,0,sizeof(viewmat));
		//z축으로 들어가기 때문에 오른손 좌표계임
		dir = -1*dir;
		//전치시킨것..
		viewmat._0_0 = right._x;
		viewmat._0_1 = right._y;
		viewmat._0_2 = right._z;

		viewmat._1_0 = Up._x;
		viewmat._1_1 = Up._y;
		viewmat._1_2 = Up._z;

		viewmat._2_0 = dir._x;
		viewmat._2_1 = dir._y;
		viewmat._2_2 = dir._z;

		viewmat._3_3 = 1.0f;
		//전치시킨거에 곱한거

		position = position * viewmat;
		position = -1 * position;

		viewmat._3_0 = position._x;
		viewmat._3_1 = position._y;
		viewmat._3_2 = position._z;
	}

	void TransformModule::TransformProjection(float a, float Short, float Long, float Degree,Matrix16f& Projection)
	{
		memset(&Projection,0,sizeof(Projection));
		//뷰평면과 뷰 원점 사이의 거리
		float d = 1 / tan((PI/180.0f * Degree)/2.0f) ;

		Projection._0_0 = d/a;
		Projection._1_1 = d;
		Projection._2_2 = (Short+Long) / (Short-Long);
		Projection._3_2 = (2*Short*Long) / (Short-Long);
		Projection._2_3 = -1;
	}
	void TransformModule::TransformMesh(Mesh* pMesh,Matrix16f mat,bool NomalizeCalc)
	{
		Vector4f v4;
		Vector3f v3;
		for(int i =0; i< pMesh->m_TriangleNum; i++)
		{
			//true 면 컬링해버림 짤라서 계산안함...
			if(pMesh->m_pBackFaceCulling[i])
				continue;
			for(int j=0; j< 3; j++)
			{
				//xyz
				v4._x = pMesh->m_pTriangleList[i].m_vt[j].m_XYZ._x;
				v4._y = pMesh->m_pTriangleList[i].m_vt[j].m_XYZ._y;
				v4._z = pMesh->m_pTriangleList[i].m_vt[j].m_XYZ._z;

				v4._w = 1.0f;
				v4 = v4*mat;
				v4._x /= v4._w;
				v4._y /= v4._w;
				v4._z /= v4._w;

				pMesh->m_pTriangleList[i].m_vt[j].m_XYZ._x = v4._x;
				pMesh->m_pTriangleList[i].m_vt[j].m_XYZ._y = v4._y;
				pMesh->m_pTriangleList[i].m_vt[j].m_XYZ._z = v4._z;
				
				if(NomalizeCalc)
				{
					//normal
					v3 = pMesh->m_pTriangleList[i].m_vt[j].m_Normal;

					v3 = v3* mat;

					pMesh->m_pTriangleList[i].m_vt[j].m_Normal= v3;
				}

			}
		}
	}
	void TransformModule::NDCToScreenMatrix(Vector2i screensize,Matrix16f& ScreenMat)
	{
		memset(&ScreenMat,0,sizeof(ScreenMat));
		ScreenMat._0_0 = (float)screensize._x / 2.f;
		ScreenMat._1_1 = (float)-(screensize._y / 2.f);

		ScreenMat._3_0 = (float)screensize._x / 2;
		ScreenMat._3_1 = (float)screensize._y / 2;

		ScreenMat._2_2 = 1.0f/2.0f;
		ScreenMat._3_2 = 1.0f/2.0f;

		ScreenMat._3_3 = 1.0f;
	}


}//end namespace JSS