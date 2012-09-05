#include "AsePaser.h"

typedef unsigned char BYTE;

CAsePaser::CAsePaser()
{
}

CAsePaser::~CAsePaser()
{
}


void CAsePaser::GetFileName(char* FullDirectory, int Strlength)
{
	char* pdest;
	int result;
	pdest = strrchr( FullDirectory, '\\' );   
	result = pdest - FullDirectory + 1;
	if(result<0)return;
	char* temp = new char[Strlength-result];
	memcpy(temp, FullDirectory+result, Strlength-result);
	memset(FullDirectory, 0, Strlength);
	memcpy(FullDirectory, temp, Strlength-result);
}

bool CAsePaser::ReadASE(char *fname,CContainer3D *pObject,CAnimationControl* pAni,JssEngine::IDevice* pDevice)
{
	FILE *fp;
	///////////////////////////파일을 열어서 없으면 실패 받는 객체가 널이면 실패
	fp = fopen(fname, "rt");
	if ( fp == NULL )
	{
		return false;
	}
	if(pObject == NULL)
		return false;
	else
		m_pContainer = pObject;
	if(pAni == NULL)
		return false;
	else
		m_pAnimationControl = pAni;
	//////////////////////////////
	m_pDevice = pDevice;//추가사항들...
	linecount = 0;
	NormalFlag = false;
	//ASE파일을 읽어서 컨테이너에 넣는다.!!!!!!!!!!!!
	DecodeASE(fp);
	
	//////////////////////////노말값계산 각 페이스당 계산한다!! 페이스 3개당 버텍스하나의 노말값을 구성!! 월드에서의 안한다면!!?
// 	if (NormalFlag == true)
// 	{
// 		MakeFaceNormalFromWorldVertex();
// 		MakeVertexNormalFromFaceNormal();
// 	}
	
	fclose(fp);

	////////////여기서부터 받아온 것들처리!!//////////////////
	m_pContainer->BuildContainer(pDevice);		//ASE파싱이 끝나면 컨테이너에서 그리기위한준비 
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CAsePaser::DecodeASE(FILE *fp)
{
	linecount++;
	int bone = 0,normal = 0;
	fgets(line, 256, fp);
	sscanf(line, "%s", string);

	// CHECK ASE FILE
	if (_stricmp(string, "*3DSMAX_ASCIIEXPORT") != 0) return -1;

	while ( !feof(fp) )
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		
		if (_stricmp(string, "*COMMENT") == 0) continue;
		
		///////////////////////
		// Scene Descriptions
		if (_stricmp(string, "*SCENE") == 0)
			DecodeSCENE(fp);
		
		///////////////////
		// material list
		if (_stricmp(string, "*MATERIAL_LIST") == 0)
			DecodeMATERIAL_LIST(fp);
		
		////////////////////
		// geometry object
		if (_stricmp(string, "*GEOMOBJECT") == 0)
		{
			CMesh* Mesh = new CMesh;
			CurMesh = Mesh;
			DecodeGEOMOBJECT(fp);
			if(strstr(CurMesh->Name,"Bip0")!=NULL)
			{
 				m_pContainer->AddBoneMesh(CurMesh);
 				bone++;
			}
			else
			{
				m_pContainer->AddMesh(CurMesh);
				normal++;
			}
		}

		if(_stricmp(string, "*HELPEROBJECT")==0)
		{
// 			CMesh* Mesh = new CMesh;
// 			CurMesh = Mesh;
// 			DecodeGEOMOBJECT(fp);
// 			m_pContainer->AddMesh(Mesh);
		}
	}
	m_pContainer->setBoneNum(bone);
	m_pContainer->setNormalNum(normal);
	return linecount;	
}

int CAsePaser::DecodeSCENE(FILE *fp)
{
	linecount += 8;
	
	fgets(line, 256, fp);//*SCENE_FILENAME	
	fgets(line, 256, fp);//*SCENE_FIRSTFRAME
		sscanf(line,"%s%f",string, &m_pAnimationControl->FirstFrame);
	fgets(line, 256, fp);//*SCENE_LASTFRAME	
		sscanf(line,"%s%f",string, &m_pAnimationControl->LastFrame);
	fgets(line, 256, fp);//*SCENE_FRAMESPEED
		sscanf(line,"%s%f",string, &m_pAnimationControl->FrameSpeed);
	fgets(line, 256, fp);//*SCENE_TICKSPERFRAME	
		sscanf(line,"%s%f",string, &m_pAnimationControl->TickPerFrame);
	fgets(line, 256, fp);//*SCENE_BACKGROUND_STATIC	
	fgets(line, 256, fp);//*SCENE_AMBIENT_STATIC	
	fgets(line, 256, fp);//}
	
	return 0;
}
	

int CAsePaser::DecodeMap(FILE* fp, CMaterial* CurMaterial)
{
	bool check;
	int i,j;
	char name[200];
	do{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line,"%s",string);
		if(_strcmpi(string,"*BITMAP")==0)
		{
			sscanf(line,"%s %s",string, name);
			memset(CurMaterial->TextureName, 0, 256);
			j=0	;
			check = false;
			memset(CurMaterial->TextureName, 0, 256);
			for(i=0; i<256; i++)
			{
				if(check)
				{
					if(line[i]=='"'){ check=false; break;}
					CurMaterial->TextureName[j] = line[i];
					j++;
				}
				if(line[i]=='"')check = true;

			}
			GetFileName(CurMaterial->TextureName, 256);
			continue;
		}
		if(_stricmp(string, "}")==0)
		{
			break;
		}
	}while(1);
	return 0;
}

int CAsePaser::DecodeMaterial(FILE* fp, CMaterial* CurMaterial)
{
	//CD3dUtil d3dutil;
	//d3dutil.InitMaterial(CurMaterial->d3dMaterial);
	int num;
	do {
		linecount++;
		fgets(line, 256, fp);
		sscanf(line,"%s",string);
		if(_strcmpi(string,"*SUBMATERIAL")==0)
		{
			sscanf(line,"%s%d",string, &num);
			CurMaterial->m_ID = num;
			DecodeMaterial(fp,&(CurMaterial->SubMaterial[num]));
			continue;
		}
		if(_strcmpi(string,"*NUMSUBMTLS")==0)
		{
			if(CurMaterial->SubMaterial)
				delete CurMaterial->SubMaterial;
			sscanf(line,"%s%d",string,&CurMaterial->SubMaterialCount);
			CurMaterial->SubMaterial = new CMaterial[CurMaterial->SubMaterialCount];
			continue;
		}
		
		if(_strcmpi(string,"*MATERIAL_AMBIENT")==0)
		{
			sscanf(line,"%s %f%f%f",string, &CurMaterial->fAmbient[0], &CurMaterial->fAmbient[1], &CurMaterial->fAmbient[2]);
			continue;
		}
		if(_strcmpi(string,"*MATERIAL_DIFFUSE")==0)
		{
			sscanf(line,"%s %f%f%f",string, &CurMaterial->fDiffuse[0], &CurMaterial->fDiffuse[1], &CurMaterial->fDiffuse[2]);
			continue;
		}
		if(_strcmpi(string, "*MATERIAL_SPECULAR")==0)
		{
			sscanf(line,"%s %f%f%f",string, &CurMaterial->fSpecular[0], &CurMaterial->fSpecular[1], &CurMaterial->fSpecular[2]);
			continue;
		}
		
		if(_strcmpi(string, "*MAP_DIFFUSE")==0)
		{
			CurMaterial->bUseTexture = true;
			CurMaterial->m_Texture = new CTextura;
			DecodeMap(fp, CurMaterial);
			continue;
		}
		if(_strcmpi(string, "*MAP_SELFILLUM")==0)
		{
// 			DecodeMap(fp, CurMaterial);
// 			continue;
		}

		if(_strcmpi(string, "*MAP_REFLECT")==0)
		{
// 			DecodeMap(fp, CurMaterial);
// 			continue;
		}


		if(_strcmpi(string, "*MAP_SPECULAR")==0)
		{
// 			DecodeMap(fp, CurMaterial);
// 			continue;
		}
		if(_stricmp(string, "}")==0)
		{
			break;
		}
		
	} while (1);

	return 0;
}
	
int CAsePaser::DecodeMATERIAL_LIST(FILE *fp)
{
	fgets(line, 256, fp);
	int num=0;
	int Temp;
	int NumMaterial;
	sscanf(line, "%s%d", string, &NumMaterial);
	m_pContainer->setMaterialNum(NumMaterial);

	m_pContainer->CreateMaterialList();

	do {
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s%d",string, &Temp);
		if(_strcmpi(string,"*MATERIAL")==0)
		{
			m_pContainer->getMaterialObject(num)->m_ID = num;
			DecodeMaterial(fp, m_pContainer->getMaterialObject(num));
			num++;
			continue;
		}
		if(_stricmp(string, "}")==0)
		{
			break;
		}
		
	} while (1);

	return 0;
}

int CAsePaser::DecodeTm(FILE* fp)
{
	float fX, fY, fZ, fW;
	MatrixIdentity(&(CurMesh->TmMatrix));
	linecount +=15;
	fgets(line, 256, fp);//*NODE_NAME
	fgets(line, 256, fp);//*INHERIT_POS
	fgets(line, 256, fp);//*INHERIT_ROT
	fgets(line, 256, fp);//*INHERIT_SCL
	fgets(line, 256, fp);//*TM_ROW0
	sscanf(line,"%s%f%f%f",string, &(CurMesh->TmMatrix._0_0),&(CurMesh->TmMatrix._1_0),&(CurMesh->TmMatrix._2_0));
	fgets(line, 256, fp);//*TM_ROW1
	sscanf(line,"%s%f%f%f",string, &(CurMesh->TmMatrix._0_1),&(CurMesh->TmMatrix._1_1),&(CurMesh->TmMatrix._2_1));
	fgets(line, 256, fp);//*TM_ROW2
	sscanf(line,"%s%f%f%f",string, &(CurMesh->TmMatrix._0_2),&(CurMesh->TmMatrix._1_2),&(CurMesh->TmMatrix._2_2));
	fgets(line, 256, fp);//*TM_ROW3
	sscanf(line,"%s%f%f%f",string, &(CurMesh->TmMatrix._0_3),&(CurMesh->TmMatrix._1_3),&(CurMesh->TmMatrix._2_3));
	fgets(line, 256, fp);//*TM_POS
	sscanf(line, "%s%f%f%f",string, &(CurMesh->TmPos._x),&(CurMesh->TmPos._y),&(CurMesh->TmPos._z));
	fgets(line, 256, fp);//*TM_ROTAXIS
	sscanf(line, "%s%f%f%f",string, &fX,&fZ,&fY);
	fgets(line, 256, fp);//*TM_ROTANGLE 
	sscanf(line, "%s%f",string, &fW);
	//fW = -fW;
	CurMesh->TmRot.x = (float)sinf(fW / 2.0f) * fX;
	CurMesh->TmRot.y = (float)sinf(fW / 2.0f) * fY;
	CurMesh->TmRot.z = (float)sinf(fW / 2.0f) * fZ;
	CurMesh->TmRot.w = (float)cosf(fW / 2.0f);
	fgets(line, 256, fp);//*TM_SCALE
	sscanf(line, "%s%f%f%f",string, &(CurMesh->TmScale._x), &(CurMesh->TmScale._y), &(CurMesh->TmScale._z));
	fgets(line, 256, fp);//*TM_SCALEAXIS
	sscanf(line, "%s%f%f%f",string, &fX,&fY,&fZ);
	fgets(line, 256, fp);//*TM_SCALEAXISANG
	sscanf(line, "%s%f",string, &fW);
	CurMesh->TmScaleRot.x = (float)sin(fW / 2.0f) * fX;
	CurMesh->TmScaleRot.y = (float)sin(fW / 2.0f) * fY;
	CurMesh->TmScaleRot.z = (float)sin(fW / 2.0f) * fZ;
	CurMesh->TmScaleRot.w = (float)cos(fW / 2.0f);
	fgets(line, 256, fp);//}
	return 0;
}

		
int CAsePaser::DecodeGEOMOBJECT(FILE *fp)
{
	int num;
	int i,j;
	bool check;
	do{
		linecount ++;
		fgets(line, 256, fp);//*NODE_NAME
		sscanf(line,"%s",string);
		if(_strcmpi(string,"*NODE_NAME")==0)
		{
			j=0	;
			check = false;
			memset(CurMesh->Name, 0, 256);
			for(i=0; i<256; i++)
			{
				if(check)
				{
					if(line[i]=='"'){ check=false; break;}
					CurMesh->Name[j] = line[i];
					j++;
				}
				if(line[i]=='"')check = true;

			}
			continue;
		}
		if(_strcmpi(string, "*NODE_PARENT")==0)
		{
			CurMesh->bParents = true;
			j=0	;
			check = false;
			memset(CurMesh->ParentName, 0, 256);
			for(i=0; i<256; i++)
			{
				if(check)
				{
					if(line[i]=='"'){ check=false; break;}
					CurMesh->ParentName[j] = line[i];
					j++;
				}
				if(line[i]=='"')check = true;

			}
			continue;
		}
		if(_strcmpi(string,"*NODE_TM")==0)
		{
			DecodeTm(fp);
			continue;
		}
		if (_stricmp(string, "*MESH") == 0)
		{
			DecodeCMesh(fp);
			continue;
		}
		if( _stricmp(string, "*MATERIAL_REF")==0)
		{
			sscanf(line, "%s %d", string, &num);
			CurMesh->MaterialRef = num;
			continue;
		}
 		if( _stricmp(string, "*TM_ANIMATION")==0)
 		{
 			DecodeANIMATION(&CurMesh->Animation, fp);
 			continue;
 		}
		if ( _stricmp(string, "}") == 0 ) 
			return 0;
	}while(1);
	return 0;
}

int CAsePaser::DecodeCMesh(FILE *fp)
{
	linecount++;
	fgets(line, 256, fp);//*TIMEVALUE
		
	int VCOUNT;
	linecount++;
	fgets(line, 256, fp);//*CMesh_NUMVERTEX
	sscanf(line, "%s%d", string, &VCOUNT);
	CurMesh->NumVertex = VCOUNT;
	///////////
	if (CurMesh->VertexList)
		delete[] CurMesh->VertexList;

	CurMesh->VertexList = new Vector3[VCOUNT];

	int TCOUNT;
	linecount++;
	fgets(line, 256, fp);//*CMesh_NUMFACES
	sscanf(line, "%s%d", string, &TCOUNT);
	///////////
	if (CurMesh->TriangleList)
		delete[] CurMesh->TriangleList;
	CurMesh->TriangleList = new TRIANGLE[TCOUNT];
	for(int i=0; i<TCOUNT; i++)
	{
		CurMesh->TriangleList[i].Init();
	}

	CurMesh->NumTriangle = TCOUNT;
	
	linecount++;
	fgets(line, 256, fp);//*CMesh_VERTEX_LIST
	sscanf(line, "%s", string);
	if ( _stricmp(string, "*MESH_VERTEX_LIST") == 0 )
		DecodeCMesh_VERTEX_LIST(fp);

	linecount++;
	fgets(line, 256, fp);//*CMesh_FACE_LIST
	sscanf(line, "%s", string);
	if ( _stricmp(string, "*MESH_FACE_LIST") == 0 )
		DecodeCMesh_FACE_LIST(fp);

	while (1)
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		if ( _stricmp(string, "*MESH_NUMTVERTEX") == 0 )//*CMesh_NUMTVERTEX
		{
			DecodeCMesh_TVERTLIST(fp);
			continue;
		}
		if(_stricmp(string, "*MESH_NUMTVFACES")==0)
		{
			DecodeCMesh_TFACELIST(fp);
			continue;
		}	
		
		if ( _stricmp(string, "*MESH_NUMCVERTEX") == 0)
		{
			DecodeCMesh_CVERTEX(fp);
			continue;
			
		}
		if(_stricmp(string,"*MESH_NUMCVFACES")==0)//*CMesh_NUMCVERTEX
		{
			DecodeCMesh_CFACELIST(fp);
			
			continue;
		}
		
		if ( _stricmp(string, "*MESH_NORMALS") == 0 )//*CMesh_NORMALS
		{
			DecodeCMesh_NORMALS(fp);
			continue;
		}

		if ( _stricmp(string, "}") == 0 ) return 0;
	}
	
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int CAsePaser::DecodeCMesh_VERTEX_LIST(FILE *fp)
{

	int Counter = 0;
	int num;
	while( Counter < CurMesh->NumVertex )
	{
		linecount++;
		fgets(line, 256, fp);//*CMesh_VERTEX
		sscanf(line, "%s%d%f%f%f", string, &num, 
			&CurMesh->VertexList[Counter]._x, 
			&CurMesh->VertexList[Counter]._y, 
			&CurMesh->VertexList[Counter]._z);
		Counter++;
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int CAsePaser::DecodeCMesh_FACE_LIST(FILE *fp)
{
	int num;
	int Counter = 0;
	char Smooth[20];
	char MaterID[20];
	int ID;
	while( Counter < CurMesh->NumTriangle )
	{
		linecount++;

		fgets(line, 256, fp);//	*CMesh_FACE  379:    A:  410 B:  408 C:  409 AB:    1
							 // BC:    1 CA:    0	 *CMesh_SMOOTHING 1 	*CMesh_MTLID 3
		memset(MaterID, 0, 20);
		memset(Smooth, 0, 20);
		sscanf(line, "%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%s%s%d", string, string,
			string, &CurMesh->TriangleList[Counter].VertexIndex[0], 
			string, &CurMesh->TriangleList[Counter].VertexIndex[1], 
			string, &CurMesh->TriangleList[Counter].VertexIndex[2],
			string, &num, string, &num, string, &num, string, Smooth,
			MaterID, &ID);
		if(_strcmpi(MaterID,"*CMesh_MTLID")==0)
		{
			CurMesh->TriangleList[Counter].MaterialID = ID;
		}
		if(_strcmpi(Smooth,"*CMesh_MTLID")==0)
		{
			CurMesh->TriangleList[Counter].MaterialID = atoi(MaterID);
		}
		Counter++;
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int CAsePaser::DecodeCMesh_TVERTLIST(FILE *fp)
{
	int MAXTVERTEX;
	sscanf(line, "%s %d", string, &MAXTVERTEX);
	if(MAXTVERTEX<=0)return 0;

	CurMesh->TVert = new TEXCOORDFLOAT[MAXTVERTEX];

	linecount++;

	float u, v, w;
	fgets(line, 256, fp);//*CMesh_TVERTLIST
	
	int counter = 0;
	int num;
	while( counter < MAXTVERTEX )
	{
		linecount++;
		fgets(line, 256, fp);//*CMesh_TVERT
		sscanf(line, "%s %d%f%f%f", string, &num, 
			&u, &v, &w);
		CurMesh->TVert[num].u = u;
		CurMesh->TVert[num].v = v;
		counter++;
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int CAsePaser::DecodeCMesh_TFACELIST(FILE *fp)
{
	int TIndex[3];

	int TFACECOUNT;
	sscanf(line, "%s %d", string, &TFACECOUNT);

	if(TFACECOUNT<=0)return 0;
	linecount++;
	fgets(line, 256, fp);//*CMesh_TFACELIST
	int Count = 0;
	int num;

	while( Count < TFACECOUNT )
	{
		linecount++;
		fgets(line, 256, fp);//*CMesh_TFACE
		sscanf(line, "%s %d %d %d %d", string, &num, 
			&TIndex[0], 
			&TIndex[1], 
			&TIndex[2]);
		if(CurMesh->TVert)
		{
			CurMesh->TriangleList[num].VertexTexture[0].u = CurMesh->TVert[ TIndex[0] ].u; 
			CurMesh->TriangleList[num].VertexTexture[0].v = CurMesh->TVert[ TIndex[0] ].v; 
			CurMesh->TriangleList[num].VertexTexture[1].u = CurMesh->TVert[ TIndex[1] ].u; 
			CurMesh->TriangleList[num].VertexTexture[1].v = CurMesh->TVert[ TIndex[1] ].v; 
			CurMesh->TriangleList[num].VertexTexture[2].u = CurMesh->TVert[ TIndex[2] ].u; 
			CurMesh->TriangleList[num].VertexTexture[2].v = CurMesh->TVert[ TIndex[2] ].v; 
		}

		Count++;
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int	CAsePaser::DecodeCMesh_CVERTEX(FILE *fp)
{

	int i,num;
	sscanf(line,"%s %d",string, &num);
	if(num<=0)return 0;
	//CurMesh->ColorList = new D3DXCOLOR[num];
	int colnum;
	float r,g,b;

	linecount++;
	fgets(line, 256, fp);
	for(i=0; i<num; i++)
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line,"%s %d%f%f%f", string, &colnum, &r, &g, &b);
	//	CurMesh->ColorList[colnum].r = r;
	//CurMesh->ColorList[colnum].g = g;
	//	CurMesh->ColorList[colnum].b = b;
//		CurMesh->ColorList[colnum].a = 1.0f;
	}
	linecount++;
	fgets(line, 256, fp);

	return 0;
}

int	CAsePaser::DecodeCMesh_CFACELIST(FILE *fp)
{
	BYTE R0, G0, B0, A0;
	int i,num;
	sscanf(line,"%s %d",string, &num);
	if(num<=0)return 0;
	int facenum, v1, v2, v3;
	linecount++;
	fgets(line, 256, fp);//*CMesh_FACENORMAL
	for(i=0; i<num; i++)
	{
		linecount++;
		fgets(line, 256, fp);//*CMesh_FACENORMAL
		sscanf(line,"%s %d %d %d %d",string, &facenum, &v1, &v2, &v3);
		//R0 = (BYTE)(CurMesh->ColorList[v1].r*255.0f);
		//G0 = (BYTE)(CurMesh->ColorList[v1].g*255.0f);
		//B0 = (BYTE)(CurMesh->ColorList[v1].b*255.0f);
		//A0 = (BYTE)(CurMesh->ColorList[v1].a*255.0f);
		//CurMesh->TriangleList[facenum].VertexColor[0] = D3DCOLOR_XRGB(R0,G0,B0);
		//R0 = (BYTE)(CurMesh->ColorList[v2].r*255.0f);
		//G0 = (BYTE)(CurMesh->ColorList[v2].g*255.0f);
		//B0 = (BYTE)(CurMesh->ColorList[v2].b*255.0f);
		//A0 = (BYTE)(CurMesh->ColorList[v2].a*255.0f);
		//CurMesh->TriangleList[facenum].VertexColor[1] = D3DCOLOR_XRGB(R0,G0,B0);
		//R0 = (BYTE)(CurMesh->ColorList[v3].r*255.0f);
		//G0 = (BYTE)(CurMesh->ColorList[v3].g*255.0f);
		//B0 = (BYTE)(CurMesh->ColorList[v3].b*255.0f);
		//A0 = (BYTE)(CurMesh->ColorList[v3].a*255.0f);
		//CurMesh->TriangleList[facenum].VertexColor[2] = D3DCOLOR_XRGB(R0,G0,B0);
	}
	linecount++;
	fgets(line, 256, fp);//*CMesh_FACENORMAL

	return 0;
}

int CAsePaser::DecodeCMesh_NORMALS(FILE *fp)
{
	int counter = 0;	
	
	NormalFlag = true;
	float x, y, z;
	int num, num2;
	NormalFlag = true;
	while ( counter < CurMesh->NumTriangle )
	{
		linecount++;
		fgets(line, 256, fp);//*CMesh_FACENORMAL
		sscanf(line, "%s%d%f%f%f", string, &num,	&x,	&y,	&z);
		CurMesh->TriangleList[num].FaceNormal._x = x;
		CurMesh->TriangleList[num].FaceNormal._y = y;
		CurMesh->TriangleList[num].FaceNormal._z = z;

		linecount++;
		fgets(line, 256, fp);//*CMesh_VERTEXNORMAL
		sscanf(line, "%s%d%f%f%f", string, &num2,	&x,	&y,	&z);
		CurMesh->TriangleList[num].VertexNormal[0]._x = x;
		CurMesh->TriangleList[num].VertexNormal[0]._y = y;
		CurMesh->TriangleList[num].VertexNormal[0]._z = z;

		linecount++;
		fgets(line, 256, fp);//*CMesh_VERTEXNORMAL
		sscanf(line, "%s%d%f%f%f", string, &num2,	&x,	&y,	&z);
		CurMesh->TriangleList[num].VertexNormal[2]._x = x;
		CurMesh->TriangleList[num].VertexNormal[2]._y = y;
		CurMesh->TriangleList[num].VertexNormal[2]._z = z;
		
		linecount++;
		fgets(line, 256, fp);//*CMesh_VERTEXNORMAL
		sscanf(line, "%s%d%f%f%f", string, &num2,	&x,	&y,	&z);
		CurMesh->TriangleList[num].VertexNormal[1]._x = x;
		CurMesh->TriangleList[num].VertexNormal[1]._y = y;
		CurMesh->TriangleList[num].VertexNormal[1]._z = z;

		counter++;
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}
		
void CAsePaser::MakeVertexNormalFromFaceNormal(void)
{

}



void CAsePaser::MakeFaceNormalFromWorldVertex(void)
{
	
	int i;
	Vector3 temp1, temp2, temp3;
	float length;
	Vector3* Vec = NULL;
	CMesh* CMesh = m_pContainer->getMeshObject();
	while(CMesh)
	{
		Vec = new Vector3[CMesh->NumVertex];
		for(i=0; i<CMesh->NumVertex; i++)
		{
			Vec[i] = Vector3(0.0f, 0.0f, 0.0f);
		}

		for ( i = 0; i < CMesh->NumTriangle; i++)
		{
			temp1._x = CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[1] ]._x 
				- CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[0] ]._x;
			temp1._y = CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[1] ]._y 
				- CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[0] ]._y;
			temp1._z = CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[1] ]._z 
				- CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[0] ]._z;

			temp2._x = CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[2] ]._x 
				- CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[0] ]._x;
			temp2._y = CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[2] ]._y 
				- CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[0] ]._y;
			temp2._z = CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[2] ]._z 
				- CMesh->VertexList[ CMesh->TriangleList[i].VertexIndex[0] ]._z;
			
			temp3._x = temp1._y * temp2._z - temp1._z * temp2._y;
			temp3._y = temp1._z * temp2._x - temp1._x * temp2._z;
			temp3._z = temp1._x * temp2._y - temp1._y * temp2._x;

			length = (float)sqrt(temp3._x * temp3._x + temp3._y * temp3._y + temp3._z * temp3._z);

			CMesh->TriangleList[i].FaceNormal._x = temp3._x / length;
			CMesh->TriangleList[i].FaceNormal._y = temp3._y / length;
			CMesh->TriangleList[i].FaceNormal._z = temp3._z / length;

			Vec[CMesh->TriangleList[i].VertexIndex[0]] += CMesh->TriangleList[i].FaceNormal;
			Vec[CMesh->TriangleList[i].VertexIndex[1]] += CMesh->TriangleList[i].FaceNormal;
			Vec[CMesh->TriangleList[i].VertexIndex[2]] += CMesh->TriangleList[i].FaceNormal;
		}
		for(i=0; i<CMesh->NumVertex; i++)
		{
			Vec3Normalize(&Vec[i], &Vec[i]);
		}
		for(i=0; i<CMesh->NumTriangle; i++)
		{
			CMesh->TriangleList[i].VertexNormal[0] = Vec[CMesh->TriangleList[i].VertexIndex[0]];
			CMesh->TriangleList[i].VertexNormal[1] = Vec[CMesh->TriangleList[i].VertexIndex[1]];
			CMesh->TriangleList[i].VertexNormal[2] = Vec[CMesh->TriangleList[i].VertexIndex[2]];
		}
		delete[] Vec;
	CMesh = CMesh->m_pNext;
	}
	
}
int CAsePaser::DecodeANIMATION(ANIMATION *Animation,FILE *fp)
{
	while (1)
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		if ( _stricmp(string, "*NODE_NAME") == 0 )
		{
			sscanf(line, "%s %s",string, Animation->Name);
			continue;
		}	
		


		if( _stricmp(string, "*CONTROL_ROT_TRACK") == 0 || _stricmp(string, "*CONTROL_ROT_TCB") == 0
			|| _stricmp(string,"*CONTROL_ROT_BEZIER")==0)
		{
			DecodeROT_TRACK(Animation,fp);
			continue;
		}
		
		if ( _stricmp(string, "*CONTROL_SCALE_TRACK") == 0 || _stricmp(string, "*CONTROL_SCALE_TCB") == 0
			|| _stricmp(string,"*CONTROL_SCALE_BEZIER")==0)
		{
			DecodeSCALE_TRACK(Animation,fp);
			continue;
		}

		if(   _stricmp(string, "*CONTROL_POS_TRACK")==0  || _stricmp(string, "*CONTROL_POS_TCB")==0
			|| _stricmp(string,"*CONTROL_POS_BEZIER")==0)
		{
			DecodePOS_TRACK(Animation,fp);
			continue;
		}

		if ( _stricmp(string, "}") == 0 ) return 0;
	}
	return 1;
}


int CAsePaser::DecodeROT_TRACK(ANIMATION *Animation,FILE *fp)
{
	ROT_TRACK* Temp;
	ROT_TRACK* Tail=NULL;
	if(Animation->m_Rot)
		delete Animation->m_Rot;
	Animation->m_Rot = NULL;
	Vector3 ReadRot;

	CQuaternion TmqR = CurMesh->TmRot;
	float fW;
	CQuaternion qR;
	while (1)
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		if ( _stricmp(string, "}") == 0 )
		{

			return 0;
		}
		
		Temp = new ROT_TRACK;
		sscanf(line, "%s%d%f%f%f%f",string, &(Temp->Index), &ReadRot._x, &ReadRot._y, &ReadRot._z, &fW);
		Temp->x = ReadRot._x;
		Temp->y = ReadRot._y;
		Temp->z = ReadRot._z;
		Temp->w = fW;

		qR.x = (float)sinf(fW / 2.0f) * ReadRot._x;
		qR.y = (float)sinf(fW / 2.0f) * ReadRot._y;
		qR.z = (float)sinf(fW / 2.0f) * ReadRot._z;
		qR.w = (float)cosf(fW / 2.0f);
		if(Tail==NULL)
		{
			Temp->qR = qR;
			Tail = Temp;
			Animation->m_Rot = Temp;
		}
		else
		{
			QuaternionMultiply(&Temp->qR, &Tail->qR, &qR);
			Tail->m_pNext = Temp;
			Temp->m_pPrev = Tail;
			Tail = Temp;
		}
	}

	return 1;
}


int CAsePaser::DecodePOS_TRACK(ANIMATION *Animation,FILE *fp)
{
	POS_TRACK* Temp;
	POS_TRACK* Tail=NULL;
	if(Animation->m_Pos)
		delete Animation->m_Pos;
	Animation->m_Pos = NULL;
	Vector3 vTemp;

	while (1)
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		if ( _stricmp(string, "}") == 0 ) return 0;
		Temp = new POS_TRACK;
		sscanf(line, "%s %d%f%f%f",string, &(Temp->Index), &(vTemp._x), &(vTemp._y), &(vTemp._z));
		Temp->Pos = vTemp;
		if(Animation->m_Pos==NULL)
		{
			Animation->m_Pos = Temp;
			Tail = Temp;
		}
		else
		{
			Tail->m_pNext = Temp;
			Temp->m_pPrev = Tail;
			Tail = Temp;
		}
	}
	return 1;
}

int CAsePaser::DecodeSCALE_TRACK(ANIMATION *Animation,FILE *fp)
{
	SCALE_TRACK* Temp;
	SCALE_TRACK* Tail=NULL;
	if(Animation->m_Scale)
		delete Animation->m_Scale;
	Animation->m_Scale = NULL;

	while (1)
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		if ( _stricmp(string, "}") == 0 ) return 0;

		Temp = new SCALE_TRACK;
		sscanf(line, "%s %d%f%f%f%f%f%f%f",string, &(Temp->Index), &(Temp->fX), &(Temp->fY), &(Temp->fZ), 
			&(Temp->aX), &(Temp->aY), &(Temp->aZ), &(Temp->Ang));
		if(Animation->m_Scale==NULL)
		{
			Animation->m_Scale = Temp;
			Tail = Temp;
		}
		else
		{
			Tail->m_pNext = Temp;
			Temp->m_pPrev = Tail;
			Tail = Temp;
		}
	}
	return 1;
}






