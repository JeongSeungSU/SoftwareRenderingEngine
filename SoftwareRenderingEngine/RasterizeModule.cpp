#include "RasterizeModule.h"

namespace JssEngine
{
	void BubbleSort(Triangle* pTriangle)
	{
		for(int i = 0; i< 3; i++)
		{
			for(int j = 0; j< 3-i-1; j++)
			{
				if(	pTriangle->m_vt[j].m_XYZ._y > pTriangle->m_vt[j+1].m_XYZ._y)
				{
					swap(pTriangle->m_vt[j],pTriangle->m_vt[j+1]);
				}
			}
		}
	}

	RasterizeModule::RasterizeModule()
	:m_pixel(NULL)
	{
	}

	RasterizeModule::~RasterizeModule(void)
	{
		m_pixel = NULL;
	}

	void RasterizeModule::SetScreenPixelPointer(void *pixel)
	{
		m_pixel = pixel;
	}
	void RasterizeModule::SetScreenSize(Vector2i ScreenSize)
	{
		m_ScreenSize = ScreenSize;
	}

	void RasterizeModule::SetPixel(int x, int y, DWORD color)
	{
		DWORD* pixel = reinterpret_cast<DWORD*>(m_pixel);

		const int sizex = m_ScreenSize._x; 
		const int sizey = m_ScreenSize._y;

		if(sizex < x || 1 > x || sizey < y || 1 > y)
			return;

		pixel[((sizey-y)* sizex)+x] = color;
	}
	
	Vector2f RasterizeModule::WeightMid(Vector2f STCool,Vector3f * pV)
	{
		Vector3f Vertex[3] = {Vector3f(pV[0]._x, pV[0]._y,0),
						      Vector3f(pV[1]._x, pV[1]._y,0),
							  Vector3f(pV[2]._x, pV[2]._y,0)};

		Vector3f vt(STCool._x,STCool._y,0);

		Vector3f U = Vertex[1] - Vertex[0];
		Vector3f V = Vertex[2] - Vertex[0];
		Vector3f W = vt  	  -  Vertex[0];		// 음 ST를 구하기위한 거시기 

		Vector2f tmp;
		float a,b,c;
		float s,t;

		a = Norm(VectorCross(U,W))/2;
		b = Norm(VectorCross(V,W))/2;
		c = Norm(VectorCross(U,V))/2;

		s = b/c;
		t = a/c;

		return Vector2f(s,t);
	}
	void RasterizeModule::SolidDraw(Triangle* pTriangle,Texture* pTexture,float** ppZBuffer)
	{
		int MaxY,MinY,MidY;
		///////////죳같네 바꾸자//////////////
		Vector3f vertex[3];
		Vector2f tex[3];

		BYTE bR[3],bG[3],bB[3];

		BubbleSort(pTriangle);
		for(int i =0 ; i< 3; i++)
		{
			vertex[i] = pTriangle->m_vt[i].m_XYZ;
			tex[i]	  = pTriangle->m_vt[i].m_UV1;
			bR[i] = A8R8G8B8ToByte(pTriangle->m_vt[i].m_Color,R);
			bG[i] = A8R8G8B8ToByte(pTriangle->m_vt[i].m_Color,G);
			bB[i] = A8R8G8B8ToByte(pTriangle->m_vt[i].m_Color,B);
			
		}
	
		MaxY = (int)vertex[2]._y;
		MidY = (int)vertex[1]._y;
		MinY = (int)vertex[0]._y;

		//자 선분의 기울기를 구해보실까요 그런데 이건 바꿀수 있음....
		float MinX, MaxX;
		float MinD, MaxD;		//작은값 x의 증분 큰값 X의 증분
		//MAX-MID : m1
		//Max-MIN : m2
		//Mid-MIN : m3
		//일단 부동소수점 연산으로 구한다 시봉 ㅋ 이걸 브리젠함으로 바꾸자
		//브리젠함 알고리즘을 따로 함수화? 하자고 

		float m1 = Degree(vertex[2]._x,vertex[2]._y,vertex[1]._x,vertex[1]._y);
		float m2 = Degree(vertex[2]._x,vertex[2]._y,vertex[0]._x,vertex[0]._y);
		float m3 = Degree(vertex[1]._x,vertex[1]._y,vertex[0]._x,vertex[0]._y);

		if(m1 > m2)
		{
			MaxD = m1;
			MinD = m2;
		}
		else
		{
			MaxD = m2;
			MinD = m1;
		}
		MinX = vertex[2]._x;
		MaxX = vertex[2]._x;
		//view-> 무게중심 좌표
		Vector3f PixeltoView;
		Vector2f WeightMidCoold;
		BYTE r,g,b;
		float u,v;
		float z;


		//이쪽에 코드 중복인데 흠 -_-;;;;;;;;
		for(int Ycount = MaxY; Ycount >= MidY; Ycount--)
		{
			for(int i = MinX ; i <= MaxX; i++)
			{
				WeightMidCoold = WeightMid(Vector2f(i,Ycount),vertex);

				u =  ((tex[1]._x - tex[0]._x)*WeightMidCoold._x) + ((tex[2]._x - tex[0]._x)*WeightMidCoold._y) + (tex[0]._x);
				v =  ((tex[1]._y - tex[0]._y)*WeightMidCoold._x) + ((tex[2]._y - tex[0]._y)*WeightMidCoold._y) + (tex[0]._y);

				z = ((vertex[1]._z - vertex[0]._z)*WeightMidCoold._x) + ((vertex[2]._z - vertex[0]._z)*WeightMidCoold._y) + (vertex[0]._z);


				if(ppZBuffer != NULL)
				{
					if(ppZBuffer[Ycount][i] <= z)
						continue;
					else
						ppZBuffer[Ycount][i] = z;
				}

				if(pTexture)
				{
					int ui = u*pTexture->m_Pixmap.nRows;
					int vi = v*pTexture->m_Pixmap.nCols;

					r = pTexture->m_Pixmap.GetPixel(ui,vi).r;
					g = pTexture->m_Pixmap.GetPixel(ui,vi).g;
					b = pTexture->m_Pixmap.GetPixel(ui,vi).b;
				}
				else
				{
					r = (BYTE)( (float)(bR[1] - bR[0]) * WeightMidCoold._x ) +(BYTE)((float)(bR[2] - bR[0]) * WeightMidCoold._y) + bR[0];
					g = (BYTE)( (float)(bG[1] - bG[0]) * WeightMidCoold._x ) +(BYTE)((float)(bG[2] - bG[0]) * WeightMidCoold._y) + bG[0];
					b = (BYTE)( (float)(bB[1] - bB[0]) * WeightMidCoold._x ) +(BYTE)((float)(bB[2] - bB[0]) * WeightMidCoold._y) + bB[0];
				}

				SetPixel(i,Ycount,ColorA8R8G8B8(0,r,g,b));
			}
			MinX += MinD;
			MaxX += MaxD;  
		}

		m2 *=-1;
		m3 *=-1;
		if(m3 > m2)
		{
			MaxD = m3;
			MinD = m2;
		}
		else
		{
			MaxD = m2;
			MinD = m3;
		}

		MinX = vertex[0]._x;
		MaxX = vertex[0]._x;
		//2분류로 나눠서 처리한다... 하지만 어떤 점이 최고 최저인지는 모르네 -_-;
		for(int Ycount = MinY; Ycount < MidY; Ycount++)
		{
			for(int i = MinX ; i <= MaxX ;i++)
			{
				//view-> 무게중심 좌표
				WeightMidCoold = WeightMid(Vector2f(i,Ycount),vertex);


				u =  ((tex[1]._x - tex[0]._x)*WeightMidCoold._x) + ((tex[2]._x - tex[0]._x)*WeightMidCoold._y) + (tex[0]._x);
				v =  ((tex[1]._y - tex[0]._y)*WeightMidCoold._x) + ((tex[2]._y - tex[0]._y)*WeightMidCoold._y) + (tex[0]._y);

				z = ((vertex[1]._z - vertex[0]._z)*WeightMidCoold._x) + ((vertex[2]._z - vertex[0]._z)*WeightMidCoold._y) + (vertex[0]._z);

				//Zbuffer가 존재하면 하고 아니면 말고..
				if(ppZBuffer != NULL)
				{
					if(ppZBuffer[Ycount][i] <= z)
						continue;
					else
						ppZBuffer[Ycount][i] = z;
				}
				
				//Texture가 있을때는 Texture를 적용하고 아니면 RGB컬러 적용 원래 블렌딩해야되는데 ㅡ,.ㅡ;;;
				if(pTexture)
				{
					int ui = u*pTexture->m_Pixmap.nRows;
					int vi = v*pTexture->m_Pixmap.nCols;

					r = pTexture->m_Pixmap.GetPixel(ui,vi).r;
					g = pTexture->m_Pixmap.GetPixel(ui,vi).g;
					b = pTexture->m_Pixmap.GetPixel(ui,vi).b;
				}
				else
				{
					r = (BYTE)( (float)(bR[1] - bR[0]) * WeightMidCoold._x ) +(BYTE)((float)(bR[2] - bR[0]) * WeightMidCoold._y) + bR[0];
					g = (BYTE)( (float)(bG[1] - bG[0]) * WeightMidCoold._x ) +(BYTE)((float)(bG[2] - bG[0]) * WeightMidCoold._y) + bG[0];
					b = (BYTE)( (float)(bB[1] - bB[0]) * WeightMidCoold._x ) +(BYTE)((float)(bB[2] - bB[0]) * WeightMidCoold._y) + bB[0];
				}


				SetPixel(i,Ycount,ColorA8R8G8B8(0,r,g,b));
			}
			MinX += MinD;
			MaxX += MaxD;
		}
	}

	void RasterizeModule::LineDraw(Vector2i v1, Vector2i v2)
	{

		//기울기가 없을떄의 처리...
		if((v1._x - v2._x) == 0)
		{
			if(v1._y > v2._y)
				swap(v1,v2);
			for(int y = v1._y; y <= v2._y ; y++)
				SetPixel(v1._x,y,ColorA8R8G8B8(0,0,0,0));

			return;
		}
		else if((v2._y - v1._y) == 0)
		{
			if(v1._x > v2._x)
				swap(v1,v2);
			for(int x = v1._x ; x <= v2._x; x++)
				SetPixel(x,v1._y,ColorA8R8G8B8(0,0,0,0));

			return;
		}

		//일단 v1과 v2의 x좌표상에서 적은것이 왼쪽 v1으로 가게 함.. 한마디로 스왑
		if(v1._x > v2._x)
			swap(v1,v2);

		int x = v1._x , y = v1._y ,x1 = v2._x , y1 = v2._y;	//이것은 좌표들 ㅋㅋ 찍을 좌표들 
		int dx,dy,d,Incx = 1,Incy = -1,incrE,incrNE,yDgree = -1,xDgree = 1, yCompare = 1;
		float m;	//기울기
		bool Inverse = false;
		//x에대한 거시기는 끝......
		//이제 기울기에 대한 거시기들 m>1일때
		if(IsZero((float)(v2._x - v1._x)))  //이건 스왑이 필요
			m = 1.1f;
		else
			m = (float)(v2._y - v1._y) / (float)(v2._x - v1._x);

		if( m > 0.0f)
		{
			Incy = 1;
			yDgree*=-1;
			yCompare *= -1;
			m *= -1.0f;
		}
		if( m < -1.0f)			//기울기가 1보다 크면 x,y자체를 스왑 -_-;
		{
			Inverse = true;
			yDgree*=-1;
			xDgree*=-1;
		}

		dx = x1 - x;
		dy = y1 - y;
		dy *= yDgree;
		dx *= xDgree;

		if( m < -1.0f)			//기울기가 1보다 크면 x,y자체를 스왑 -_-;
		{
			d = dy - dx*2;

			incrE = 2*(dy-dx);
			incrNE = -(2*dx);

			SetPixel(x,y,ColorA8R8G8B8(0,0,0,0));
			while(yCompare*y > yCompare*y1)
			{
				if(d >=0)
				{
					d+= incrE;
					x+=Incx;
					y+=Incy;
				}
				else
				{
					d+= incrNE;
					y += Incy;
				}
				SetPixel(x,y,ColorA8R8G8B8(0,0,0,0));
			}
		}
		else
		{
			d = dy*2 - dx;
			incrE = dy*2;
			incrNE = (dy-dx)*2;

			SetPixel(x,y,ColorA8R8G8B8(0,0,0,0));
			while(x < x1)
			{
				if(d <=0)
				{
					d+= incrE;
					x+=Incx;
				}
				else
				{
					d+= incrNE;
					x+=Incx;
					y += Incy;
				}
				SetPixel(x,y,ColorA8R8G8B8(0,0,0,0));
			}
		}
	}
}