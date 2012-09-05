#pragma once
#include "AtomicClass.h"
#include <string>
#include <iostream>
#include "SoftwareRendering.h"
using namespace std;

//[정승수]
//BMP에서 RGB성분을 뽑아내기 위한 자료구조!!
class RGBmap
{

public:
	unsigned char r,g,b;	//각각 1BYTE 로 구성
};

//BMP 를 가져오기 위한 자료구조
class RGBpixmap
{
public:
	RGBpixmap()
	{
		nRows = 0;
		nCols = 0;
		pixel = 0;
	}
	~RGBpixmap()
	{
		if(pixel != NULL)
		{
			delete [] pixel;
			pixel = NULL;
		}
	}

public:
	int nRows, nCols;
	RGBmap *pixel;
	int readBMPFile(char * fname);
	void setTexture(int textureName);
};

//텍스쳐 OPENGL에 등록하기 위한 ID 파일 이름 을 가진다.
class CTextura
{
	int m_ID;
	string m_FileName;
	RGBpixmap m_Pixmap;

	static int m_Last_ID;

public:
	CTextura();
	void setID(int id) {  m_ID = id; };
	bool setFilename(string FileName);

	void Init();

	int getID() { return m_ID; };
	string getFileName() {return m_FileName;}

	void ApplyTextura(JssEngine::IDevice* device);
	void DisableTextura(JssEngine::IDevice* device);

	int getHeight() { return m_Pixmap.nRows;};
	int getWidth() { return m_Pixmap.nCols;};
};
//이것은 전체적인 재질 정보/
class CMaterial
{
public:
	int  m_ID;							//ASE에서의 ID
	bool bUseTexture;					//텍스쳐 사용유무
	char TextureName[256];				//텍스쳐 이름
	int SubMaterialCount;				//서브 메테리얼의 갯수
	CMaterial* SubMaterial;				//서브메테리얼 정보

	float	fAmbient[3];
	float	fDiffuse[3];
	float	fSpecular[3];

	CTextura* m_Texture;				//텍스쳐

public:
	CMaterial():
	bUseTexture(false),
		SubMaterialCount(0),
		SubMaterial(NULL),
		m_Texture(NULL)
	{
	}
	~CMaterial()
	{
		if(SubMaterial)
			delete[] SubMaterial;
		if(m_Texture)
			delete m_Texture;
	}
public:
	void BuildTexture(JssEngine::IDevice* device);			//텍스쳐 생성!!
	void apply(JssEngine::IDevice* device);

	void aplicarMaterial();
	CTextura* getTexture() { return m_Texture;}
};

