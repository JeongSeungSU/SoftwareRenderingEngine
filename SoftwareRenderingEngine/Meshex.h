#pragma once
#include "AtomicClass.h"
#include "Materialex.h"
#include "SoftwareRendering.h"

//[���¼�] �� �޽� ����
class CMesh
{
public:
	int	 m_iView;			//���° ������ ����Ʈ�� ������ִ�??����
	char Name[256];			//�޽��̸�
	bool bParents;			//�θ��� ����
	char ParentName[256];	//�θ��� �̸�
	int NumVertex;			//���ؽ� ����
	int NumTriangle;		//�ﰢ�� face ����
	Vector3* VertexList;	//���ؽ� ����Ʈ!!
	TRIANGLE* TriangleList;	//�� ����Ʈ!!
	Matrix16 TmMatrix;		//���� ��Ʈ����!
	Matrix16 InvTm;			//�� ���� ��Ʈ���� �����

	CTextura*					m_pMyTexture;			//�ؽ��ĸ� ����...
	JssEngine::VertexBuffer*	m_pVertexBuffer;		//���ؽ� ����.. �ؽ��ĳ� ���ó� �� ��.. �̸��ٲ�߰ڳ� ..
	JssEngine::IDevice*			m_pDeivce;

	Vector3 TmPos;			//��ġ
	CQuaternion TmRot;		//ȸ�� ����� ����
	Vector3 TmScale;		//ũ��
	CQuaternion TmScaleRot;	//���ʹϾ�
	TEXCOORDFLOAT* TVert;	//�ؽ��� ��ǥ
	
	CMesh* m_pNext;			//���� �Ž�!!
	CMesh* m_pParents;		//�θ� �޽�
	int MaterialRef;		//�� �Ž��� ����Ǵ� ���� ��ȣ!!
	

	ANIMATION Animation;	//���ϸ��̼� ���� �ڷᱸ����
	MeshContainer* m_pChilds;	//�� �޽��� �ڽ�!!

	Matrix16 CalcMatrix;	//����� ��Ʈ����!

public:
	CMesh()
		:NumVertex(0),
		NumTriangle(0),
		VertexList(NULL),
		TriangleList(NULL),
		m_pNext(NULL),
		MaterialRef(-1),
		TVert(NULL),
		m_pChilds(NULL),
		bParents(false),
		m_pParents(NULL),
		m_pMyTexture(NULL),
		m_pVertexBuffer(NULL)
	{
	}
	~CMesh()
	{
		if(VertexList)
			delete[] VertexList;
		VertexList = NULL;
		if(TriangleList)
			delete[] TriangleList;
		TriangleList = NULL;
		if(m_pNext)delete m_pNext;
		if(TVert)delete[] TVert;
		if(m_pVertexBuffer) m_pVertexBuffer->ReleaseBuffer();
		if(m_pChilds) delete m_pChilds;
	}

	void draw();
	void AddChild(CMesh* pMesh);
	void AnimateMatrix(Matrix16* matParents, float CurFrame,bool stop);		//��� �޽��� �ִϸ��̼� ������ ����!!!�ƴ� ��纯ȯ�� ����!!


	void SetTexture(CTextura* Tex) {m_pMyTexture = Tex;}
	CTextura* GetTexture(){return m_pMyTexture;}

	void SetDevice(JssEngine::IDevice * Device) {m_pDeivce = Device;}


	
};