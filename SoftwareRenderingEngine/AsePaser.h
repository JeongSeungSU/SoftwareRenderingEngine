#pragma once
#include <string>
#include "SoftwareRendering.h"
#include "Meshex.h"
#include "Container3D.h"
#include "Animation.h"

using namespace std;

//[정승수] ASE파일을 불러와 컨테이너와 애니메이션 객체에 넣어준다!!

class CAsePaser
{

public:
	CAsePaser();
	virtual ~CAsePaser();
	//라인 그 스트링 함수!! 라인카운트!!	노말있냐없냐!!
	char line[256];
	char string[80];
	int linecount;
	bool NormalFlag;

	JssEngine::IDevice*	m_pDevice;			//softwareRendering Device~~JSSENGINE

	//이 객체하나로 모든 디코딩!!
	CMesh* CurMesh;

	//파싱하여 정보를 저장할 컨테이너
	CContainer3D* m_pContainer;
	CAnimationControl* m_pAnimationControl;
public:
	//파싱!!
	bool ReadASE(char *fname,CContainer3D *pObject,CAnimationControl* pAni,JssEngine::IDevice* pDevice);

protected:
	//모두 ASE정보를 각각 읽기 위한 함수들
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

