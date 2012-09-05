#pragma once
#include "AtomicClass.h"
#include <string>
#include <iostream>
#include "SoftwareRendering.h"
using namespace std;

//[���¼�]
//BMP���� RGB������ �̾Ƴ��� ���� �ڷᱸ��!!
class RGBmap
{

public:
	unsigned char r,g,b;	//���� 1BYTE �� ����
};

//BMP �� �������� ���� �ڷᱸ��
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

//�ؽ��� OPENGL�� ����ϱ� ���� ID ���� �̸� �� ������.
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
//�̰��� ��ü���� ���� ����/
class CMaterial
{
public:
	int  m_ID;							//ASE������ ID
	bool bUseTexture;					//�ؽ��� �������
	char TextureName[256];				//�ؽ��� �̸�
	int SubMaterialCount;				//���� ���׸����� ����
	CMaterial* SubMaterial;				//������׸��� ����

	float	fAmbient[3];
	float	fDiffuse[3];
	float	fSpecular[3];

	CTextura* m_Texture;				//�ؽ���

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
	void BuildTexture(JssEngine::IDevice* device);			//�ؽ��� ����!!
	void apply(JssEngine::IDevice* device);

	void aplicarMaterial();
	CTextura* getTexture() { return m_Texture;}
};

