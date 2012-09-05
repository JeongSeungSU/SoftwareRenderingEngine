#pragma once
#include "Meshex.h"

//[정승수] 3D 정보를 담는 컨테이너 객체 
/*
1. 재질 정보 
2. 메쉬 정보 
3. 보여줄 리스트 
4. 본 정보 

ASE파서에서 이객체를 가져와 데이터를 넣어준다.
또한 애니메이션 객체에 의해 컨트롤 당한다.
자기 자신을 그릴줄 안다.
*/
class CContainer3D
{
private:
	CMesh*			m_MeshList;		//링크드리스트 헤더들
	CMesh*			m_BoneList;		//그니까 얘네들을 포함하고 있다고 생각하면됨
	CMaterial*      m_MaterialList;	//재질의 헤더

	int  m_iNumMaterial;			//재질 갯수
	int  m_iNumBoneMesh;			//본 재질 갯수
	int  m_iNumNormalMesh;			//보통 재질 갯수..

	bool m_bInit;

	ViewList m_ViewList;			//보여질 리스트
	ViewList m_ViewBoneList;

	float*  m_pFloatMatrix;			//OpenGl로 변환하기 위한 임시...

	void   MakeInherite();			//계층구조 자식과 부모관계를 정의한다.
	CMesh* Search(char* name);		//메쉬의 이름을 받아 메쉬포인터를 반환
	CMesh* SearchBone(char* name);
	void   SetAllMesh();			//ASE파서는 모든 버텍스 정보는 이미 월드 좌표계에서 뽑아온다.
									//이것은 TM부분에서 월드변환 매트릭스의 역행렬을 구해서 모든 정점정보들을
									//로컬좌표계로 변환시켜준다. 애니메이션을 위한 행동

	bool   m_bControl;


	JssEngine::IDevice*	m_pDeivce;
public:
	CContainer3D(void);
	~CContainer3D(void);
public:
	void BuildContainer(JssEngine::IDevice* pDevice);			//자료를 받아왔다면 OPENGL과 연동하여 버텍스정보를 넣고 재질과 합쳐 그릴 준비를 한다.
	
	void AddMesh(CMesh* Mesh);		
	void AddBoneMesh(CMesh* Mesh);

		
	void Animation(Matrix16 mat, float frame);		//애니메이션 재귀 함수로써 최초의 부모가 없는 노드들부터 모든 자식을 업데이트 시킴
	void BoneAnimation(Matrix16 mat, float frame);	//위에는 그려지는 메쉬에 대한 정보업데이트고 이건 본에 대한 정보

	void setMaterialNum(int num ) { m_iNumMaterial = num ;}
	int  getMaterialNum() { return m_iNumMaterial ;}
	void setNormalNum(int num ) { m_iNumNormalMesh = num ;}
	void setBoneNum(int num ) { m_iNumBoneMesh = num ;}

	CMaterial*  getMaterialObject(int num = 0);
	CMesh*		getMeshObject(int num = 0);

	void CreateMaterialList(int num = 0);

	void SetControl (bool set){	m_bControl = set;}

	void draw();
};

