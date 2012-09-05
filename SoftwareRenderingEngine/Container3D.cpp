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
	//������ �ʼ�...?
	if(m_pFloatMatrix == NULL)
		m_pFloatMatrix = new float[16];

	MakeInherite();					//�������� �θ��ڽ� ���踦 �����Ѵ�.
	Matrix16 mat;
	MatrixIdentity(&mat);			

	CMesh* CurMesh;
	CurMesh = m_MeshList;
	while(CurMesh)					//��� �޽� ����Ʈ�� ���鼭 ��������� �ʱ�ȭ ������
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

	//SetAllMesh();					//����->����

	CMaterial* culmaterial = m_MaterialList;	//��� ������ ���鼭 �ؽ��İ� ������ �ε��Ѵ�.
	for(int i = 0; i< m_iNumMaterial; i++)
	{
		getMaterialObject(i)->BuildTexture(pDevice);
	}
	
	//���⼭ OPENGL�� �־��ش�!!!!!!!!!
	//////////////////
	CMesh* CullMesh = m_MeshList;				//OPENGL DISPLAY LIST�� �־ ����
	while( CullMesh != NULL )
	{

// 	int base = glGenLists(1);		//�׷��� ����Ʈ ũ�� ���� 
// 	m_ViewList.push_back(base);		//������ ����Ʈ ����!!!!!!
// 	CullMesh->m_iView = m_ViewList.size() - 1;

// 	glNewList(base, GL_COMPILE);	//���ο� ����Ʈ �ۼ�!!
// 		glPushMatrix();
		
		CTextura* t = NULL;
		bool textura = false;
		//���� �Էºκ�!!!
		DWORD fvf = JSSFVF_XYZ | JSSFVF_TEX0;
	
	float* pVertexBuffer;
	m_pDeivce->GetVideoManager()->CreateVertexBuffer(CullMesh->NumTriangle*5*3*sizeof(float),fvf,&CullMesh->m_pVertexBuffer);
	CullMesh->m_pVertexBuffer->Lock(0,CullMesh->NumTriangle*5*3*sizeof(float),(void**)&pVertexBuffer);
	
	

		for (int i = 0; i < CullMesh->NumTriangle; i++)				//���� ������ŭ ������ ����.
		{
			if (getMaterialNum() > 0)								//���������� ���ٸ� �н�
			{
				//������׸��� ����
				if(getMaterialObject(CullMesh->MaterialRef) == NULL)
					t= NULL;
				else
					t = getMaterialObject(CullMesh->MaterialRef)->getTexture();
				if (t != NULL)
				{
					if(!textura)
						CullMesh->SetTexture(t);
					textura = true;			//�ؽ��� ���
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





// 			glBegin( GL_TRIANGLES);	//�׸��� ��� �� 3���� ������ �ϳ��� �ﰢ���� �����ϴ� ���!!
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

	m_bInit = true;			//�ʱ�ȭ �Ϸ�!!
	////�������/////////////
	return;
}

void CContainer3D::MakeInherite()	//�������� �����
{
	CMesh* Child;
	CMesh* Parents;
	Child = m_MeshList;
	while(Child)									//�ڽ��� �ִٸ� �θ� �ڽ��� �־��ش�.
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

	Child = m_BoneList;								//�̰��� ���� ���� ��������!!
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
void CContainer3D::SetAllMesh()							//���� ���ߵ��� ����->���� �� ��� ���ؽ����� ��ȯ
{
	CMesh* Mesh = m_MeshList;
	int i,j;
	Vector3 vTemp;
	Vector3 vTemp2;
	Matrix16 mat;
	while(Mesh)
	{
		MatrixInverse(&mat,  &(Mesh->TmMatrix));		//ASE���� �޾ƿ� ���� ��Ʈ������ ����Ľ�Ų��.
		Mesh->InvTm = mat;								//�װ��� �־��ְ�
		for(i=0; i<Mesh->NumVertex; i++)				//��� ���ؽ���
		{
			vTemp = Mesh->VertexList[i];
			Vec3TransformCoord(&vTemp2, &vTemp, &mat);	//���ؽ� * ������ ������� ���־� ������ǥ��� ��ȭ��Ų��.
			Mesh->VertexList[i] = vTemp2;

		}
		Mesh = Mesh->m_pNext;
	}
	Mesh = m_BoneList;									//���� ��������
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

void CContainer3D::AddMesh(CMesh* Mesh)			//�޽�����Ʈ ���� 
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

void CContainer3D::AddBoneMesh(CMesh* Mesh)		//������Ʈ ���� ��ũ�帮��Ʈ����
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

void CContainer3D::Animation(Matrix16 mat, float frame)				//�ִϸ��̼� ����
{
	CMesh* CurMesh = m_MeshList;

	while(CurMesh)
	{
		if(m_BoneList)												//���� �ִٸ�!!
		{	
			if(CurMesh->bParents)									//�θ� �ִٸ�!!�θ��� ��������� ������� �ϹǷ� ���ڷ� �ְ� ���� �������� �־��ش�.
				CurMesh->AnimateMatrix(&CurMesh->m_pParents->CalcMatrix,frame,m_bControl);
			else
				CurMesh->AnimateMatrix(&mat,frame,m_bControl);
		}
		else														//���� ���ٸ� 
		{
			if(!CurMesh->bParents)
				CurMesh->AnimateMatrix(&mat,frame,m_bControl);
		}
		CurMesh = CurMesh->m_pNext;
	}
}

void CContainer3D::BoneAnimation(Matrix16 mat, float frame)			//�������� �ִϸ��̼�
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
// 		glRotated(-90,1.0,0.0,0.0);							//ASE�� �״�� xyz������ -90�� x��������� ȸ��������� ����� ���´�.
// 		glScaled(60,60,60);									//�ʹ� �۾Ƽ� 60�� Ȯ�� -0-;;
// 		
// 		ParserInMatrix(m_pFloatMatrix,cullMesh->CalcMatrix);	
// 		glMultMatrixf(m_pFloatMatrix);						//PUSH POP�� ���´�� ��Ʈ������ �̿��� ���������� ����!
// 		
// 		glCallList(m_ViewList[cullMesh->m_iView]);			//�׷��� ����Ʈ���� �׸�����Ʈ�� �ҷ���
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