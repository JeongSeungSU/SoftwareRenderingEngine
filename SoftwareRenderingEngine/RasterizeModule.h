#pragma once

#include "Mathematics.h"
#include "Triangle.h"
#include "Material.h"

namespace JssEngine
{
	class RasterizeModule
	{
	private:
		void *		m_pixel;
		Vector2i	m_ScreenSize;

		Vector2f WeightMid(Vector2f STCool,Vector3f * pV);

		

	public:
		RasterizeModule();
		~RasterizeModule(void);
	public:
		void SolidDraw(Triangle* pTriangle,Texture* pTexture,float** ppZBuffer);
		void LineDraw(Vector2i v1, Vector2i v2);

		void SetPixel(int x, int y, DWORD color);


		void SetScreenPixelPointer(void *pixel);
		void SetScreenSize(Vector2i ScreenSize);
	};
}