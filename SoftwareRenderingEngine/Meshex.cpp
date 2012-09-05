#include "Meshex.h"


void CMesh::draw()
{
	
}
void CMesh::AddChild(CMesh* pMesh)		//자식이 들어올시 컨테이너를 생성해 링크드리스트 제작
{
	MeshContainer* pTemp;
	pTemp = new MeshContainer;
	pTemp->pData = pMesh;
	if(m_pChilds==NULL)
	{
		m_pChilds = pTemp;
	}
	else 
	{
		m_pChilds->Add(pTemp);
	}

}
void CMesh::AnimateMatrix(Matrix16* matParents, float CurFrame,bool stop)	//애니메이션 /
{

	MatrixIdentity(&CalcMatrix);							//일단 최종행렬 초기화
	CQuaternion qR;
	CQuaternion qR1;
	Vector3 Trans;
	Vector3 Trans1;
	Matrix16 mat2, mat3;
	ROT_TRACK* Rt = Animation.GetRotTrack(CurFrame);			//현재프레임에 가장가까운 ROT변환을 내놓음
	//	SCALE_TRACK* St = Animation.GetScaleTrack(CurFrame);

	if(Rt)									//변환이 있다면!!
	{
		qR = Rt->qR;						//회전에 관련된 사원수를 저장!

		if(Rt->m_pNext)						//1과 2사이의 보간을 위한 다음 사원수를 뺀다!!
		{
			qR1 = Rt->m_pNext->qR;
			QuaternionSlerp(&qR, &qR, &qR1,							//구면 선형 보간 함수 !! 선형보간시 갑자기 빨라지고 느려지는 부분이 있다.!!
				(CurFrame - (float)Rt->Index)/(float)(Rt->m_pNext->Index - Rt->Index));	//그래서 구면선형 보간을 채용 일정한 속도를 낼수 있다.!!
		}

		MatrixIdentity(&mat2);							//초기화
		MatrixRotationQuaternion(&mat2, &qR);			//매트릭스에 사원수변환한 것을 옮기

	}
	else
	{

		if(m_pParents)							//변환이 없다면 걍 부모의 역행렬과 곱
		{
			mat2 =  m_pParents->InvTm *TmMatrix;
			mat2._0_3 = 0;
			mat2._1_3 = 0;
			mat2._2_3 = 0;
			mat2._3_3 = 1.0f;
		}
		else
		{
			mat2 = TmMatrix;						//부모마저 없다면 그냥 자기자신의 월드행렬!
			mat2._0_3 = 0;
			mat2._1_3 = 0;
			mat2._2_3 = 0;
			mat2._3_3 = 1.0f;
		}

	}
													//위에는 회전에 관한 보간 이것은 이동에 관한 것!!
	POS_TRACK* Pt = Animation.GetPosTrack(CurFrame);
	if(Pt)
	{
		Trans = Pt->Pos; 

		if(Pt->m_pNext)
		{
			Trans1 = Pt->m_pNext->Pos; 
			Trans1 = ((CurFrame - (float)Pt->Index)/(float)(Pt->m_pNext->Index - Pt->Index))*(Trans1 - Trans);	//이동벡터의 선형보간 -0-설명할필요가..
			Trans = Trans + Trans1;
		}
	}
	else
	{
		if(m_pParents)
		{
			mat3 = m_pParents->InvTm*TmMatrix ;		//마찬가지로 변환이 없다면
			Trans._x = mat3._0_3;
			Trans._y = mat3._1_3;
			Trans._z = mat3._2_3;
		}
		else
		{
			Trans =TmPos;
		}
	}


	mat2._0_3 = Trans._x;
	mat2._1_3 = Trans._y;
	mat2._2_3 = Trans._z;

	if(stop)
		CalcMatrix = (*matParents) * mat2  ;			//부모의 총변환 행렬에 이제 자기자신의 변환한 행렬을 곱해서 계층구조를 생성!!
	else
		CalcMatrix =  TmMatrix * CalcMatrix;	//애니메이션죽일시 그냥 자기자신의 월드변환을 곱해주는 방식!!
	MeshContainer* pTemp = m_pChilds;
	while(pTemp)
	{
		pTemp->pData->AnimateMatrix(&CalcMatrix, CurFrame,stop); //자식이 있다면 모두 업데이트 해준다.!!
		pTemp = pTemp->m_pNext;
	}
}
