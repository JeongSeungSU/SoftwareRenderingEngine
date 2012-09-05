#include "Texture.h"
#include <fstream>

namespace JssEngine
{

	

	unsigned short getShort(FILE *inf)
	{
		char ic;
		unsigned short ip;

		ic = getc(inf); ip = ic;
		ic = getc(inf); ip |= ((unsigned short)ic << 8);

		return ip;
	}

	unsigned long getLong(FILE *inf)
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
	int RGBpixmap::readBMPFile(const char * fname)
	{
		int k, row, col, numPadBytes, nBytesInRow, count;
		char c1, c2, dum, r, g, b;
		unsigned long fileSize, offBits, headerSize, numCols, numRows, compression, imageSize;
		unsigned long xPels, yPels, numLUTentries, impColors;
		unsigned short reserved, planes, bitsPerPixel;
		
		std::FILE *inf;
		inf = fopen(fname, "r+b");					
		if (inf == NULL)
			return 0;

		c1 = getc(inf);	c2 = getc(inf);

		fileSize = getLong(inf);			//���� �о� ���� ����Ʈ�� �������� �޾Ƶ���
		reserved = getShort(inf);	
		reserved = getShort(inf);	
		offBits = getLong(inf);	
		headerSize = getLong(inf); 
		numCols = getLong(inf);
		numRows = getLong(inf);

		planes = getShort(inf);	
		bitsPerPixel = getShort(inf);	
		compression = getLong(inf);
		imageSize = getLong(inf);
		xPels = getLong(inf);
		yPels = getLong(inf);
		numLUTentries = getLong(inf);
		impColors = getLong(inf);


		nBytesInRow = ((3*numCols+3)/4)*4;		
		numPadBytes = nBytesInRow - 3 * numCols;
		nRows = numRows;							
		nCols = numCols;

		pixel = new RGBmap[nRows * nCols];		//RGB���� ���� ���μ��� ũ�� ��ŭ �����!!

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


	RGBmap RGBpixmap::GetPixel(int row, int col)
	{
		RGBmap map;
		
		if(row < 0)
			row = 0;
		if(row > nRows-1)
			row = nRows-1;

		if(col < 0)
			col = 0;
		if(col > nCols-1)
			col = nCols-1;

		map.r = pixel[(row) + (col*nCols)].r;
		map.g = pixel[(row) + (col*nCols)].g;
		map.b = pixel[(row) + (col*nCols)].b;
 
		return map;
	}


	Texture::Texture()
	{ 
		Init();
	}

	void Texture::Init()
	{
		m_ID = -1;
		strcpy(m_FileName,"");
	}

	bool Texture::setFilename(const char* filename)
	{
		strcpy(m_FileName,filename);
		if (strcmp(m_FileName,"") == 0 || (m_ID == -1))
			return false;

		m_Pixmap.readBMPFile(filename);									//��Ʈ�� ���� �ε�
		setID(m_ID);													//�ؽ��� ���̵� ����

		return true;
	}


}