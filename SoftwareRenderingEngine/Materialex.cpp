#include "Materialex.h"
#include <fstream>

FILE *inf;
int CTextura::m_Last_ID = 1;			//�ؽ��� ID����� ���� 


void RGBpixmap::setTexture(int textureName)
{
// 	glBindTexture(GL_TEXTURE_2D, textureName);	//n�� �ؽ����� ����
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );			//ũ�Ⱑ �ȸ����� Ȯ��!! ���� ����
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );			//�̰� �ݴ�� ���!!
// 	//RGB������ ���� ���� ���ΰ� nRows,nCols�� �� RGB�� 1BYTE�� ���� �ȼ����� 
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nCols, nRows, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);	

}

unsigned short getShort()
{
	char ic;
	unsigned short ip;

	ic = getc(inf); ip = ic;
	ic = getc(inf); ip |= ((unsigned short)ic << 8);
	
	return ip;
}

unsigned long getLong()
{
	unsigned long ip=0;
	char ic=0;
	unsigned char uc = ic;

	ic = getc(inf);	uc = ic;	ip = uc;
	ic = getc(inf);	uc = ic;	ip |= ((unsigned long)uc << 8);				//1byte�� 8��Ʈ�Ƿ� 8��Ʈ�� �о ����!! or => |�� �Ἥ ����!
	ic = getc(inf);	uc = ic;	ip |= ((unsigned long)uc << 16);
	ic = getc(inf);	uc = ic;	ip |= ((unsigned long)uc << 24);

	return ip;
}

//Bmp���� �б�
int RGBpixmap::readBMPFile(char * fname)
{
	int k, row, col, numPadBytes, nBytesInRow, count;
	char c1, c2, dum, r, g, b;
	unsigned long fileSize, offBits, headerSize, numCols, numRows, compression, imageSize;
	unsigned long xPels, yPels, numLUTentries, impColors;
	unsigned short reserved, planes, bitsPerPixel;

	inf = fopen(fname, "r+b");					
	if (inf == NULL)
		return 0;

	c1 = getc(inf);	c2 = getc(inf);

	fileSize = getLong();			//���� �о� ���� ����Ʈ�� �������� �޾Ƶ���
	reserved = getShort();	
	reserved = getShort();	
	offBits = getLong();	
	headerSize = getLong(); 
	numCols = getLong();
	numRows = getLong();

	planes = getShort();	
	bitsPerPixel = getShort();	
	compression = getLong();
	imageSize = getLong();
	xPels = getLong();
	yPels = getLong();
	numLUTentries = getLong();
	impColors = getLong();


	nBytesInRow = ((3*numCols+3)/4)*4;		
	numPadBytes = nBytesInRow - 3 * numCols;
	nRows = numRows;							
	nCols = numCols;

	pixel = new RGBmap[3 * nRows * nCols];		//RGB���� ���� ���μ��� ũ�� * 3 ��ŭ �����!!

	count = 0;

	for(row=0; row<nRows; row++)
	{
		for(col=0; col<nCols; col++)
		{
			b = getc(inf);	g = getc(inf);	r = getc(inf);			//����RGB�� �о�� �ȼ��ʿ� ����!!
			pixel[count].r = r;
			pixel[count].g = g;
			pixel[count++].b = b;
		}
		for(k=0; k<numPadBytes; k++)
			dum = fgetc(inf);
	}

	fclose(inf);

	return 1;	
}




CTextura::CTextura()
{ 
	Init();
}

void CTextura::Init()
{
	m_ID = m_Last_ID++;
	m_FileName = "";
}

bool CTextura::setFilename(string filename)
{
	if ((m_FileName != "") || (m_ID == -1))
		return false;
	
	m_FileName = filename;
//	m_Pixmap.readBMPFile(const_cast<char*>(filename.c_str()));		//��Ʈ�� ���� �ε�
	m_Pixmap.setTexture(m_ID);										//�ؽ��� ���̵� ����

	return true;
}

void CTextura::ApplyTextura(JssEngine::IDevice* device)
{
	if (m_ID == -1)
		return;

// 	glEnable(GL_TEXTURE_2D);							//
// 	glBindTexture(GL_TEXTURE_2D, m_ID);					//���� ����س��� �ؽ��ĸ� ������.!! �� �Ž������鿡

	device->GetVideoManager()->BindTexture(m_ID);
}

void CTextura::DisableTextura(JssEngine::IDevice* device)
{
	if (m_ID == -1)
		return;

// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glDisable(GL_TEXTURE_2D);
}

void CMaterial::BuildTexture(JssEngine::IDevice* device)
{
	if(m_Texture == NULL)
		return;
//	m_Texture->Init();
	m_Texture->setFilename(TextureName);
	device->GetVideoManager()->CreateTexture(m_Texture->getID(),m_Texture->getFileName().c_str());

	for(int i = 0 ; i< SubMaterialCount; i++)
		SubMaterial[i].BuildTexture(device);
}
void CMaterial::apply(JssEngine::IDevice* device)
{
	m_Texture->ApplyTextura(device);
}
void CMaterial::aplicarMaterial()
{
// 	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  fSpecular);		//���׸��� ���� ������ ���� x!!
// 	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   fDiffuse);
// 	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   fAmbient);
	
}