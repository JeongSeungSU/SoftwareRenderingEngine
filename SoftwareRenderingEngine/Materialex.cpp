#include "Materialex.h"
#include <fstream>

FILE *inf;
int CTextura::m_Last_ID = 1;			//텍스쳐 ID등록을 위한 


void RGBpixmap::setTexture(int textureName)
{
// 	glBindTexture(GL_TEXTURE_2D, textureName);	//n번 텍스쳐의 정보
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );			//크기가 안맞을시 확대!! 선형 으로
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );			//이건 반대로 축소!!
// 	//RGB정보를 쓰는 가로 세로가 nRows,nCols인 각 RGB당 1BYTE를 쓰는 픽셀정보 
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
	ic = getc(inf);	uc = ic;	ip |= ((unsigned long)uc << 8);				//1byte는 8비트므로 8비트씩 밀어서 저장!! or => |를 써서 저장!
	ic = getc(inf);	uc = ic;	ip |= ((unsigned long)uc << 16);
	ic = getc(inf);	uc = ic;	ip |= ((unsigned long)uc << 24);

	return ip;
}

//Bmp파일 읽기
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

	fileSize = getLong();			//각각 읽어 들인 바이트로 정보들을 받아들임
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

	pixel = new RGBmap[3 * nRows * nCols];		//RGB맵을 각각 가로세로 크기 * 3 만큼 만든다!!

	count = 0;

	for(row=0; row<nRows; row++)
	{
		for(col=0; col<nCols; col++)
		{
			b = getc(inf);	g = getc(inf);	r = getc(inf);			//각각RGB를 읽어와 픽셀맵에 저장!!
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
//	m_Pixmap.readBMPFile(const_cast<char*>(filename.c_str()));		//비트맵 파일 로딩
	m_Pixmap.setTexture(m_ID);										//텍스쳐 아이디 세팅

	return true;
}

void CTextura::ApplyTextura(JssEngine::IDevice* device)
{
	if (m_ID == -1)
		return;

// 	glEnable(GL_TEXTURE_2D);							//
// 	glBindTexture(GL_TEXTURE_2D, m_ID);					//위에 등록해놨던 텍스쳐를 입힌다.!! 각 매쉬정보들에

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
// 	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  fSpecular);		//메테리얼 지정 하지만 구현 x!!
// 	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   fDiffuse);
// 	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   fAmbient);
	
}