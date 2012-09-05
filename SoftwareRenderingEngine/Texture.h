#pragma once
#include <iostream>

namespace JssEngine
{
	enum INTENALFORMAT{GL_RGB, GL_ARGB};

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
		int readBMPFile(const char * fname);
		RGBmap GetPixel(int row, int col);
	};


	class Texture
	{
	public:
		int			m_ID;
		char		m_FileName[255];
		RGBpixmap	m_Pixmap;


	public:
		Texture();
		void setID(int id) {  m_ID = id; };
		bool setFilename(const char* FileName);

		void Init();

		int getID() { return m_ID; };

		int getHeight() { return m_Pixmap.nRows;};
		int getWidth() { return m_Pixmap.nCols;};
	};
}