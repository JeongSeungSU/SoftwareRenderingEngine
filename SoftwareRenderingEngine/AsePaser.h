#pragma once
#include <string>
#include "SoftwareRendering.h"
#include "Meshex.h"
#include "Container3D.h"
#include "Animation.h"

using namespace std;

//[���¼�] ASE������ �ҷ��� �����̳ʿ� �ִϸ��̼� ��ü�� �־��ش�!!

class CAsePaser
{

public:
	CAsePaser();
	virtual ~CAsePaser();
	//���� �� ��Ʈ�� �Լ�!! ����ī��Ʈ!!	�븻�ֳľ���!!
	char line[256];
	char string[80];
	int linecount;
	bool NormalFlag;

	JssEngine::IDevice*	m_pDevice;			//softwareRendering Device~~JSSENGINE

	//�� ��ü�ϳ��� ��� ���ڵ�!!
	CMesh* CurMesh;

	//�Ľ��Ͽ� ������ ������ �����̳�
	CContainer3D* m_pContainer;
	CAnimationControl* m_pAnimationControl;
public:
	//�Ľ�!!
	bool ReadASE(char *fname,CContainer3D *pObject,CAnimationControl* pAni,JssEngine::IDevice* pDevice);

protected:
	//��� ASE������ ���� �б� ���� �Լ���
	void GetFileName(char* FullDirectory, int Strlength);
	int DecodeASE(FILE *fp);
	int DecodeSCENE(FILE *fp);
	int DecodeMATERIAL_LIST(FILE *fp);
	int DecodeGEOMOBJECT(FILE *fp);
	int DecodeCMesh(FILE *fp);
	int DecodeCMesh_VERTEX_LIST(FILE *fp);
	int DecodeCMesh_FACE_LIST(FILE *fp);
	int DecodeCMesh_TVERTLIST(FILE *fp);
	int DecodeCMesh_TFACELIST(FILE *fp);
	int	DecodeCMesh_CVERTEX(FILE *fp);
	int	DecodeCMesh_CFACELIST(FILE *fp);
	int DecodeCMesh_NORMALS(FILE *fp);
	int DecodeMaterial(FILE* fp, CMaterial* CurMaterial);
	int DecodeMap(FILE* fp, CMaterial* CurMaterial);
	int DecodeTm(FILE* fp);
	int DecodeSCALE_TRACK(ANIMATION *Animation,FILE *fp);
	int DecodePOS_TRACK(ANIMATION *Animation,FILE *fp);
	int DecodeROT_TRACK(ANIMATION* Animation,FILE *fp);
	int DecodeANIMATION(ANIMATION* Animation,FILE *fp);
	void MakeVertexNormalFromFaceNormal(void);
	void MakeFaceNormalFromWorldVertex(void);
};

