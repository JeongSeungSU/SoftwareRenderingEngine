#pragma once
#include "AtomicClass.h"
#include "Materialex.h"
#include "SoftwareRendering.h"

//[정승수] 각 메쉬 정보
class CMesh
{
public:
	int	 m_iView;			//몇번째 보여질 리스트에 저장되있니??ㅋㅋ
	char Name[256];			//메쉬이름
	bool bParents;			//부모의 유무
	char ParentName[256];	//부모의 이름
	int NumVertex;			//버텍스 갯수
	int NumTriangle;		//삼각형 face 갯수
	Vector3* VertexList;	//버텍스 리스트!!
	TRIANGLE* TriangleList;	//면 리스트!!
	Matrix16 TmMatrix;		//월드 메트릭스!
	Matrix16 InvTm;			//위 월드 메트릭스 역행렬

	CTextura*					m_pMyTexture;			//텍스쳐만 적용...
	JssEngine::VertexBuffer*	m_pVertexBuffer;		//버텍스 버퍼.. 텍스쳐나 뭐시나 다 됨.. 이름바꿔야겠네 ..
	JssEngine::IDevice*			m_pDeivce;

	Vector3 TmPos;			//위치
	CQuaternion TmRot;		//회전 사원수 적용
	Vector3 TmScale;		//크기
	CQuaternion TmScaleRot;	//쿼터니언
	TEXCOORDFLOAT* TVert;	//텍스쳐 좌표
	
	CMesh* m_pNext;			//다음 매쉬!!
	CMesh* m_pParents;		//부모 메쉬
	int MaterialRef;		//이 매쉬의 적용되는 재질 번호!!
	

	ANIMATION Animation;	//에니메이션 관련 자료구조임
	MeshContainer* m_pChilds;	//이 메쉬의 자식!!

	Matrix16 CalcMatrix;	//계산할 매트릭스!

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
	void AnimateMatrix(Matrix16* matParents, float CurFrame,bool stop);		//모든 메쉬의 애니메이션 정보를 갱신!!!아니 모든변환을 갱신!!


	void SetTexture(CTextura* Tex) {m_pMyTexture = Tex;}
	CTextura* GetTexture(){return m_pMyTexture;}

	void SetDevice(JssEngine::IDevice * Device) {m_pDeivce = Device;}


	
};