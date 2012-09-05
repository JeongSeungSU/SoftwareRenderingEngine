#pragma once
#include "Video.h"
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

namespace JssEngine
{
	class SoftwareVideoManager : public VideoManager
	{
	private:
		HWND	m_HWND;

		HDC m_HDC;
		HDC hMemDC;
		HBITMAP hBitmap, OldBitmap;
		HPEN hPen,OldPen;

		BITMAP CurrentScreen;

		void* m_pBits;

		
		void BeginDrawBefore();
		void EndDrawBefore();
		
		void Rasterize(Mesh* pMesh);

		#ifdef DEBUGDATA
		char	m_chDebugData[8][255];
		void	PrintDebugData();
		void	InitDebugData();
		#endif
	public:
		SoftwareVideoManager(HWND	hWnd,Vector2i	ScreenSize);
		~SoftwareVideoManager(void);
	
	public:
		void DrawPrimited(PRIMITE_TYPE type, int Start,int Number);

		void FIllRECT();

	};
}