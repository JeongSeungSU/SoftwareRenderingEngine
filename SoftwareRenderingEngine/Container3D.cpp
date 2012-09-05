#include "Container3D.h"


CContainer3D::CContainer3D(void)
{
	m_bInit = false;
	m_pFloatMatrix = NULL;
	m_MeshList = NULL;
	m_BoneList = NULL;
	m_MaterialList = NULL;
	m_iNumMaterial = 0;
	m_iNumBoneMesh = 0;
	m_iNumNormalMesh = 0;
	m_bControl = false;
	
}


CContainer3D::~CContainer3D(void)
{
	if(m_pFloatMatrix != NULL)
		delete[] m_pFloatMatrix;
	m_pFloatMatrix = NULL;

	if(m_MeshList != NULL)
		delete m_MeshList;
	m_MeshList = NULL;
	if(m_BoneList != NULL)
		delete m_BoneList;
	m_BoneList = NULL;

	if(m_MaterialList != NULL)
		delete[] m_MaterialList;

}
void CContainer3D::BuildContainer(JssEngine::IDevice* pDevice)
{
	m_pDeivce = pDevice;
	//ㅋㅋㅋ 필수...?
	if(m_pFloatMatrix == NULL)
		m_pFloatMatrix = new float[16];

	MakeInherite();					//계층구조 부모자식 관계를 정의한다.
	Matrix16 mat;
	MatrixIdentity(&mat);			

	CMesh* CurMesh;
	CurMesh = m_MeshList;
	while(CurMesh)					//모든 메쉬 리스트를 돌면서 최종행렬을 초기화 시켜줌
	{
		MatrixIdentity(&CurMesh->CalcMatrix);
		CurMesh = CurMesh->m_pNext;
	}
	CurMesh = m_BoneList;
	while(CurMesh)
	{
		MatrixIdentity(&CurMesh->CalcMatrix);
		CurMesh = CurMesh->m_pNext;
	}

	//SetAllMesh();					//월드->로컬

	CMaterial* culmaterial = m_MaterialList;	//모든 재질을 돌면서 텍스쳐가 있으면 로드한다.
	for(int i = 0; i< m_iNumMaterial; i++)
	{
		getMaterialObject(i)->BuildTexture(pDevice);
	}
	
	//여기서 OPENGL에 넣어준다!!!!!!!!!
	//////////////////
	CMesh* CullMesh = m_MeshList;				//OPENGL DISPLAY LIST에 넣어서 관리
	while( CullMesh != NULL )
	{

// 	int base = glGenLists(1);		//그려질 리스트 크기 지정 
// 	m_ViewList.push_back(base);		//보여질 리스트 저장!!!!!!
// 	CullMesh->m_iView = m_ViewList.size() - 1;

// 	glNewList(base, GL_COMPILE);	//새로운 리스트 작성!!
// 		glPushMatrix();
		
		CTextura* t = NULL;
		bool textura = false;
		//재질 입력부분!!!
		DWORD fvf = JSSFVF_XYZ | JSSFVF_TEX0;
	
	float* pVertexBuffer;
	m_pDeivce->GetVideoManager()->CreateVertexBuffer(CullMesh->NumTriangle*5*3*sizeof(float),fvf,&CullMesh->m_pVertexBuffer);
	CullMesh->m_pVertexBuffer->Lock(0,CullMesh->NumTriangle*5*3*sizeof(float),(void**)&pVertexBuffer);
	
	

		for (int i = 0; i < CullMesh->NumTriangle; i++)				//면의 갯수만큼 루프를 돈다.
		{
			if (getMaterialNum() > 0)								//재질정보가 없다면 패스
			{
				//서브메테리얼 제외
				if(getMaterialObject(CullMesh->MaterialRef) == NULL)
					t= NULL;
				else
					t = getMaterialObject(CullMesh->MaterialRef)->getTexture();
				if (t != NULL)
				{
					if(!textura)
						CullMesh->SetTexture(t);
					textura = true;			//텍스쳐 등록
				}
			}

			for(int j =0; j< 3; j++)
			{
				//xyz
				pVertexBuffer[(j*5)+(i*15)] = CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[j] ]._x;
				pVertexBuffer[(j*5)+(i*15)+1] = CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[j] ]._y;
				pVertexBuffer[(j*5)+(i*15)+2] = CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[j] ]._z;
				//uv
				pVertexBuffer[(j*5)+(i*15)+3] = CullMesh->TriangleList[i].VertexTexture[j].u;
				pVertexBuffer[(j*5)+(i*15)+4] = CullMesh->TriangleList[i].VertexTexture[j].v;
			}





// 			glBegin( GL_TRIANGLES);	//그리는 방식 각 3개의 정점당 하나의 삼각형을 구성하는 방식!!
// 
// 
// 			glNormal3f(CullMesh->TriangleList[i].VertexNormal[0]._x, CullMesh->TriangleList[i].VertexNormal[0]._y, CullMesh->TriangleList[i].VertexNormal[0]._z);
// 			glTexCoord2f(CullMesh->TriangleList[i].VertexTexture[0].u,CullMesh->TriangleList[i].VertexTexture[0].v);
// 			glVertex3f(CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[0] ]._x,CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[0] ]._y
// 					   ,CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[0] ]._z);
// 
// 			glNormal3f(CullMesh->TriangleList[i].VertexNormal[1]._x, CullMesh->TriangleList[i].VertexNormal[1]._y, CullMesh->TriangleList[i].VertexNormal[1]._z);
// 			glTexCoord2f(CullMesh->TriangleList[i].VertexTexture[1].u,CullMesh->TriangleList[i].VertexTexture[1].v);
// 			glVertex3f(CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[1] ]._x,CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[1] ]._y
// 						,CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[1] ]._z);
// 
// 			glNormal3f(CullMesh->TriangleList[i].VertexNormal[2]._x, CullMesh->TriangleList[i].VertexNormal[2]._y, CullMesh->TriangleList[i].VertexNormal[2]._z);
// 			glTexCoord2f(CullMesh->TriangleList[i].VertexTexture[2].u,CullMesh->TriangleList[i].VertexTexture[2].v);
// 			glVertex3f(CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[2] ]._x,CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[2] ]._y
// 						,CullMesh->VertexList[ CullMesh->TriangleList[i].VertexIndex[2] ]._z);
// 
// 			glEnd();



		}
		CullMesh->m_pVertexBuffer->Unlock();
	//	glPopMatrix();

//	glEndList();

	CullMesh = CullMesh->m_pNext;
	}

	m_bInit = true;			//초기화 완료!!
	////여기까지/////////////
	return;
}

void CContainer3D::MakeInherite()	//계층구조 만들기
{
	CMesh* Child;
	CMesh* Parents;
	Child = m_MeshList;
	while(Child)									//자식이 있다면 부모 자식을 넣어준다.
	{
		if(Child->bParents)
		{
			Parents = Search(Child->ParentName);
			if(Parents)
			{
				Child->m_pParents = Parents;
				Parents->AddChild(Child);
			}
			else
			{
				Parents = SearchBone(Child->ParentName);
				Child->m_pParents = Parents;
			}
		}
		Child = Child->m_pNext;
	}

	Child = m_BoneList;								//이것은 본에 관한 계층구조!!
	while(Child)
	{
		if(Child->bParents)
		{
			Parents = SearchBone(Child->ParentName);
			if(Parents)
			{
				Child->m_pParents = Parents;
				Parents->AddChild(Child);
			}
		}
		Child = Child->m_pNext;
	}
}
CMesh* CContainer3D::Search(char* name)
{
	CMesh* CMesh = m_MeshList;
	while(CMesh)
	{
		if(_stricmp(name, CMesh->Name)==0)return CMesh;
		CMesh = CMesh->m_pNext;
	}
	return NULL;
}

CMesh* CContainer3D::SearchBone(char* name)
{
	CMesh* CMesh = m_BoneList;
	while(CMesh)
	{
		if(_stricmp(name, CMesh->Name)==0)return CMesh;
		CMesh = CMesh->m_pNext;
	}
	return NULL;
}
void CContainer3D::SetAllMesh()							//위에 말했듯이 월드->로컬 로 모든 버텍스들을 변환
{
	CMesh* Mesh = m_MeshList;
	int i,j;
	Vector3 vTemp;
	Vector3 vTemp2;
	Matrix16 mat;
	while(Mesh)
	{
		MatrixInverse(&mat,  &(Mesh->TmMatrix));		//ASE에서 받아온 월드 매트릭스를 역행렬시킨다.
		Mesh->InvTm = mat;								//그것을 넣어주고
		for(i=0; i<Mesh->NumVertex; i++)				//모든 버텍스에
		{
			vTemp = Mesh->VertexList[i];
			Vec3TransformCoord(&vTemp2, &vTemp, &mat);	//버텍스 * 월드의 역행렬을 해주어 로컬좌표계로 변화시킨다.
			Mesh->VertexList[i] = vTemp2;

		}
		Mesh = Mesh->m_pNext;
	}
	Mesh = m_BoneList;									//본도 마찬가지
	while(Mesh)
	{
		MatrixInverse(&mat,  &(Mesh->TmMatrix));
		Mesh->InvTm = mat;
		for(i=0; i<Mesh->NumVertex; i++)
		{
			vTemp = Mesh->VertexList[i];
			Vec3TransformCoord(&vTemp2, &vTemp, &mat);
			Mesh->VertexList[i] = vTemp2;

		}
		Mesh = Mesh->m_pNext;
	}
}

void CContainer3D::AddMesh(CMesh* Mesh)			//메쉬리스트 생성 
{
	CMesh* temp;
	if(m_MeshList==NULL)
	{
		m_MeshList = Mesh;
	}
	else
	{
		temp = m_MeshList;
		while(temp)
		{
			if(temp->m_pNext==NULL)
			{
				temp->m_pNext = Mesh;
				break;
			}
			temp = temp->m_pNext;

		}
	}

}

void CContainer3D::AddBoneMesh(CMesh* Mesh)		//본리스트 생성 링크드리스트구조
{
	CMesh* temp;
	if(m_BoneList==NULL)
	{
		m_BoneList = Mesh;
	}
	else
	{
		temp = m_BoneList;
		while(temp)
		{
			if(temp->m_pNext==NULL)
			{
				temp->m_pNext = Mesh;
				break;
			}
			temp = temp->m_pNext;

		}
	}
}

void CContainer3D::CreateMaterialList(int num )
{
	int number;
	if(num == 0)
		number = m_iNumMaterial;
	else
		number = num;

	m_MaterialList = new CMaterial[number];
}
CMaterial*  CContainer3D:: getMaterialObject(int num)
{ 
	if(num == -1 || m_iNumMaterial - 1 < num)
		return NULL;
	else
		return &m_MaterialList[num]; 
}
CMesh*	CContainer3D::getMeshObject(int num)
{
	if( (m_iNumNormalMesh+m_iNumBoneMesh) -1 < num)
		return NULL;
	else
		return &m_MeshList[num];
}

void CContainer3D::Animation(Matrix16 mat, float frame)				//애니메이션 진행
{
	CMesh* CurMesh = m_MeshList;

	while(CurMesh)
	{
		if(m_BoneList)												//본이 있다면!!
		{	
			if(CurMesh->bParents)									//부모가 있다면!!부모의 최종행렬을 곱해줘야 하므로 인자로 넣고 현재 프레임을 넣어준다.
				CurMesh->AnimateMatrix(&CurMesh->m_pParents->CalcMatrix,frame,m_bControl);
			else
				CurMesh->AnimateMatrix(&mat,frame,m_bControl);
		}
		else														//본이 없다면 
		{
			if(!CurMesh->bParents)
				CurMesh->AnimateMatrix(&mat,frame,m_bControl);
		}
		CurMesh = CurMesh->m_pNext;
	}
}

void CContainer3D::BoneAnimation(Matrix16 mat, float frame)			//본에대한 애니메이션
{
	CMesh* CurMesh = m_BoneList;

	while(CurMesh)
	{
		if(!CurMesh->bParents)
			CurMesh->AnimateMatrix(&mat,frame,m_bControl);
		CurMesh = CurMesh->m_pNext;
	}
}

void CContainer3D::draw()
{
	CMesh* cullMesh = m_MeshList;
	JssEngine::Matrix16f mat;


	DWORD	fvf = JSSFVF_XYZ | JSSFVF_TEX0;
	while(cullMesh != NULL)
	{
// 		glPushMatrix();
// 
// 		glRotated(-90,1.0,0.0,0.0);							//ASE로 그대로 xyz뽑으면 -90도 x축방향으로 회전시켜줘야 제대로 나온다.
// 		glScaled(60,60,60);									//너무 작아서 60배 확대 -0-;;
// 		
// 		ParserInMatrix(m_pFloatMatrix,cullMesh->CalcMatrix);	
// 		glMultMatrixf(m_pFloatMatrix);						//PUSH POP을 쓰는대신 매트릭스를 이용한 계층구조를 실현!
// 		
// 		glCallList(m_ViewList[cullMesh->m_iView]);			//그려진 리스트에서 그릴리스트를 불러옴
// 		glPopMatrix();

// 		if(cullMesh->MaterialRef !=5)
// 		{
// 			cullMesh = cullMesh->m_pNext;
// 			continue;
// 		}
		if(cullMesh->m_pMyTexture)
			m_pDeivce->GetVideoManager()->BindTexture(cullMesh->m_pMyTexture->getID());

		m_pDeivce->GetVideoManager()->SetVertexBuffer(cullMesh->m_pVertexBuffer);
		
		m_pDeivce->GetVideoManager()->SetFVF(fvf);

		m_pDeivce->GetVideoManager()->DrawPrimited(JssEngine::JSS_PRIMITE_TRIANGLE,0,cullMesh->NumTriangle*3);


		cullMesh = cullMesh->m_pNext;
	}
}