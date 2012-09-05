#pragma once
#include "Meshex.h"

//[���¼�] 3D ������ ��� �����̳� ��ü 
/*
1. ���� ���� 
2. �޽� ���� 
3. ������ ����Ʈ 
4. �� ���� 

ASE�ļ����� �̰�ü�� ������ �����͸� �־��ش�.
���� �ִϸ��̼� ��ü�� ���� ��Ʈ�� ���Ѵ�.
�ڱ� �ڽ��� �׸��� �ȴ�.
*/
class CContainer3D
{
private:
	CMesh*			m_MeshList;		//��ũ�帮��Ʈ �����
	CMesh*			m_BoneList;		//�״ϱ� ��׵��� �����ϰ� �ִٰ� �����ϸ��
	CMaterial*      m_MaterialList;	//������ ���

	int  m_iNumMaterial;			//���� ����
	int  m_iNumBoneMesh;			//�� ���� ����
	int  m_iNumNormalMesh;			//���� ���� ����..

	bool m_bInit;

	ViewList m_ViewList;			//������ ����Ʈ
	ViewList m_ViewBoneList;

	float*  m_pFloatMatrix;			//OpenGl�� ��ȯ�ϱ� ���� �ӽ�...

	void   MakeInherite();			//�������� �ڽİ� �θ���踦 �����Ѵ�.
	CMesh* Search(char* name);		//�޽��� �̸��� �޾� �޽������͸� ��ȯ
	CMesh* SearchBone(char* name);
	void   SetAllMesh();			//ASE�ļ��� ��� ���ؽ� ������ �̹� ���� ��ǥ�迡�� �̾ƿ´�.
									//�̰��� TM�κп��� ���庯ȯ ��Ʈ������ ������� ���ؼ� ��� ������������
									//������ǥ��� ��ȯ�����ش�. �ִϸ��̼��� ���� �ൿ

	bool   m_bControl;


	JssEngine::IDevice*	m_pDeivce;
public:
	CContainer3D(void);
	~CContainer3D(void);
public:
	void BuildContainer(JssEngine::IDevice* pDevice);			//�ڷḦ �޾ƿԴٸ� OPENGL�� �����Ͽ� ���ؽ������� �ְ� ������ ���� �׸� �غ� �Ѵ�.
	
	void AddMesh(CMesh* Mesh);		
	void AddBoneMesh(CMesh* Mesh);

		
	void Animation(Matrix16 mat, float frame);		//�ִϸ��̼� ��� �Լ��ν� ������ �θ� ���� ������� ��� �ڽ��� ������Ʈ ��Ŵ
	void BoneAnimation(Matrix16 mat, float frame);	//������ �׷����� �޽��� ���� ����������Ʈ�� �̰� ���� ���� ����

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

