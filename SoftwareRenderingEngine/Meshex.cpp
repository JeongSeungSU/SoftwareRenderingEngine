#include "Meshex.h"


void CMesh::draw()
{
	
}
void CMesh::AddChild(CMesh* pMesh)		//�ڽ��� ���ý� �����̳ʸ� ������ ��ũ�帮��Ʈ ����
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
void CMesh::AnimateMatrix(Matrix16* matParents, float CurFrame,bool stop)	//�ִϸ��̼� /
{

	MatrixIdentity(&CalcMatrix);							//�ϴ� ������� �ʱ�ȭ
	CQuaternion qR;
	CQuaternion qR1;
	Vector3 Trans;
	Vector3 Trans1;
	Matrix16 mat2, mat3;
	ROT_TRACK* Rt = Animation.GetRotTrack(CurFrame);			//���������ӿ� ���尡��� ROT��ȯ�� ������
	//	SCALE_TRACK* St = Animation.GetScaleTrack(CurFrame);

	if(Rt)									//��ȯ�� �ִٸ�!!
	{
		qR = Rt->qR;						//ȸ���� ���õ� ������� ����!

		if(Rt->m_pNext)						//1�� 2������ ������ ���� ���� ������� ����!!
		{
			qR1 = Rt->m_pNext->qR;
			QuaternionSlerp(&qR, &qR, &qR1,							//���� ���� ���� �Լ� !! ���������� ���ڱ� �������� �������� �κ��� �ִ�.!!
				(CurFrame - (float)Rt->Index)/(float)(Rt->m_pNext->Index - Rt->Index));	//�׷��� ���鼱�� ������ ä�� ������ �ӵ��� ���� �ִ�.!!
		}

		MatrixIdentity(&mat2);							//�ʱ�ȭ
		MatrixRotationQuaternion(&mat2, &qR);			//��Ʈ������ �������ȯ�� ���� �ű�

	}
	else
	{

		if(m_pParents)							//��ȯ�� ���ٸ� �� �θ��� ����İ� ��
		{
			mat2 =  m_pParents->InvTm *TmMatrix;
			mat2._0_3 = 0;
			mat2._1_3 = 0;
			mat2._2_3 = 0;
			mat2._3_3 = 1.0f;
		}
		else
		{
			mat2 = TmMatrix;						//�θ��� ���ٸ� �׳� �ڱ��ڽ��� �������!
			mat2._0_3 = 0;
			mat2._1_3 = 0;
			mat2._2_3 = 0;
			mat2._3_3 = 1.0f;
		}

	}
													//������ ȸ���� ���� ���� �̰��� �̵��� ���� ��!!
	POS_TRACK* Pt = Animation.GetPosTrack(CurFrame);
	if(Pt)
	{
		Trans = Pt->Pos; 

		if(Pt->m_pNext)
		{
			Trans1 = Pt->m_pNext->Pos; 
			Trans1 = ((CurFrame - (float)Pt->Index)/(float)(Pt->m_pNext->Index - Pt->Index))*(Trans1 - Trans);	//�̵������� �������� -0-�������ʿ䰡..
			Trans = Trans + Trans1;
		}
	}
	else
	{
		if(m_pParents)
		{
			mat3 = m_pParents->InvTm*TmMatrix ;		//���������� ��ȯ�� ���ٸ�
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
		CalcMatrix = (*matParents) * mat2  ;			//�θ��� �Ѻ�ȯ ��Ŀ� ���� �ڱ��ڽ��� ��ȯ�� ����� ���ؼ� ���������� ����!!
	else
		CalcMatrix =  TmMatrix * CalcMatrix;	//�ִϸ��̼����Ͻ� �׳� �ڱ��ڽ��� ���庯ȯ�� �����ִ� ���!!
	MeshContainer* pTemp = m_pChilds;
	while(pTemp)
	{
		pTemp->pData->AnimateMatrix(&CalcMatrix, CurFrame,stop); //�ڽ��� �ִٸ� ��� ������Ʈ ���ش�.!!
		pTemp = pTemp->m_pNext;
	}
}
