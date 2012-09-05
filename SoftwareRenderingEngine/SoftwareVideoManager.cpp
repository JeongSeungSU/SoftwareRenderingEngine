#include "SoftwareVideoManager.h"

namespace JssEngine
{
	SoftwareVideoManager::SoftwareVideoManager(HWND	hWnd,Vector2i	ScreenSize)
	:VideoManager(ScreenSize),m_HWND(hWnd)
	{
	
		m_HDC = GetDC(m_HWND);
		hMemDC = CreateCompatibleDC(m_HDC);


		BITMAPINFO bmInfo;
		ZeroMemory(&bmInfo, sizeof(BITMAPINFO)); 
		bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
		bmInfo.bmiHeader.biWidth = m_ScreenSize._x;
		bmInfo.bmiHeader.biHeight = m_ScreenSize._y;
		bmInfo.bmiHeader.biPlanes = 1;
		bmInfo.bmiHeader.biBitCount = 32; // ���� ������ ���� �ݵ�� 32�� ���� 
		bmInfo.bmiHeader.biCompression = BI_RGB;
		bmInfo.bmiHeader.biSizeImage = 0; // MSDN������ BI_RGB �ɼ��� ��� 0���� �����϶�� �Ǿ� �ִ�.
		bmInfo.bmiHeader.biClrUsed = 0;
		bmInfo.bmiHeader.biClrImportant = 0;

		// ��Ʈ�� �ڵ� ���ο� �����ϴ� �������� ��Ʈ�� ������ �ּҴ�. 
		hBitmap = CreateDIBSection(NULL, &bmInfo, DIB_RGB_COLORS, &m_pBits, NULL, 0L);

		OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		memset(m_pBits, 0, m_ScreenSize._x * m_ScreenSize._y * 4); // ������ �ּҸ� ���� �޸� �＼���� ���� �۾��� ��� 
		// ũ������ DIB ���� ���Ŀ� UpdateLayeredWindow ���� ���̿������ ������ �۾��ص� ����.
		// ����Ʈ ������Ʈ�� �����μ��� �ǹ̷� �ۿ��ϴ� �� ����. ���� �Ҵ�� pBits�� ��ȭ�� ����. 
		// ���� �޸� DC�� ���� �׸� �� �ִٸ� �޸� DC�� ����Ʈ ������Ʈ �� �� �׸��� ���� ����.

		//�ϴ� ��⿡��...
		m_RasterizeModule.SetScreenSize(m_ScreenSize);
		m_RasterizeModule.SetScreenPixelPointer(m_pBits);
	}
	void SoftwareVideoManager::FIllRECT()
	{
		RECT rc;

		rc.left =0;
		rc.top = 0;
		rc.right = m_ScreenSize._x;
		rc.bottom = m_ScreenSize._y;

		FillRect( hMemDC, &rc, GetSysColorBrush( COLOR_WINDOW ));
	}
	SoftwareVideoManager::~SoftwareVideoManager(void)
	{

		DeleteObject(SelectObject(hMemDC,OldBitmap));
		DeleteDC(hMemDC);
		ReleaseDC(m_HWND,m_HDC);


	}
	#ifdef DEBUGDATA
	void	SoftwareVideoManager::PrintDebugData()
	{
		int height = 15;
		int starty = 10;
		static DWORD time = 0;
		static int	 frame = 0;

		//FILLMODE
		if(m_DebugData.m_FillMode == JSS_FILL_WIRE)
			sprintf(m_chDebugData[0],"%s : %s","FILL MODE", "WIRE MODE");
		else
			sprintf(m_chDebugData[0],"%s : %s","FILL MODE", "SOLID MODE");




		//ZBUFFER
		if(m_DebugData.m_bZbuffer)
			sprintf(m_chDebugData[1],"%s : %s","Z-BUFFER", "ON");
		else
			sprintf(m_chDebugData[1],"%s : %s","Z-BUFFER", "OFF");


		//CULLING
		if(m_DebugData.m_BackfaceMode == JSS_CULL_CCW)
			sprintf(m_chDebugData[2],"%s : %s","CULLING", "CCW(�ĸ� �ø�)");
		else if(m_DebugData.m_BackfaceMode == JSS_CULL_CW)
			sprintf(m_chDebugData[2],"%s : %s","CULLING", "CW(�ո� �ø�)");
		else
			sprintf(m_chDebugData[2],"%s : %s","CULLING", "NONE(�ø�����)");

		//TextureNum
		sprintf(m_chDebugData[3],"%s : %d","Texture", m_DebugData.m_iTextureNum);


		//MeshNum
		sprintf(m_chDebugData[4],"%s : %d","MeshNum", m_DebugData.m_iMeshNum);


		//TRIANGLENUM
		sprintf(m_chDebugData[5],"%s : %d","TriangleNum", m_DebugData.m_iTriangleNum);


		//VertexNum
		sprintf(m_chDebugData[6],"%s : %d","VertexNum", m_DebugData.m_iVertexNum);


		//frame
		sprintf(m_chDebugData[7],"%s : %d","FRAME", m_DebugData.m_Frame);



		if(timeGetTime() - time < 1000)
			frame++;
		else
		{
			time = timeGetTime();
			m_DebugData.m_Frame = frame;
			frame = 0;
		}

		for(int i= 0; i<8 ; i++)
		{
			TextOut(hMemDC,10,starty,m_chDebugData[i],strlen(m_chDebugData[i]));
			starty += height;
		}

	}
	void	SoftwareVideoManager::InitDebugData()
	{
		m_DebugData.m_BackfaceMode	= m_ApplyModule.m_CULLMODE;
		m_DebugData.m_bZbuffer		= m_ApplyModule.m_Zbuffering;
		m_DebugData.m_FillMode		= m_ApplyModule.m_FillMode;
		m_DebugData.m_iMeshNum		= 0;
		m_DebugData.m_iTextureNum	= m_TextureMap.size();
		m_DebugData.m_iTriangleNum	= 0;
		m_DebugData.m_iVertexNum	= 0;
	}
	#endif

	void SoftwareVideoManager::DrawPrimited(PRIMITE_TYPE type, int Start,int Number)
	{
		//�ʱ�ȭ
		Mesh* pMesh			 = CreateBufferToMesh(m_NowFVF,m_pNowVertexBuffer,type,Start,Number);
		pMesh->m_MaterialNum = m_NowMaterial;	//���׸��� �ѹ���...

		#ifdef DEBUGDATA
		m_DebugData.m_iMeshNum++;
		#endif

		//�ٷ� �׷���...
		FixedPipeLine(pMesh);
		
		delete pMesh;
		pMesh =	NULL;
	}
	void SoftwareVideoManager::BeginDrawBefore()
	{
		RECT rc;

		rc.left =0;
		rc.top = 0;
		rc.right = m_ScreenSize._x;
		rc.bottom = m_ScreenSize._y;
		memset(m_pBits,255,sizeof(DWORD) * m_ScreenSize._x * m_ScreenSize._y);

		m_ppZBuffer = new float*[m_ScreenSize._x];
		m_ppZBuffer[0] = new float[m_ScreenSize._x*m_ScreenSize._y];

		for(int i =0; i<m_ScreenSize._x; i++)
		{
			m_ppZBuffer[i] = (m_ppZBuffer[0]+m_ScreenSize._y*i);
		}
		for(int i =0; i<m_ScreenSize._x* m_ScreenSize._y; i++)
			m_ppZBuffer[i/m_ScreenSize._x][i%m_ScreenSize._x] = 1.0f;

		
		#ifdef DEBUGDATA
		InitDebugData();
		#endif
	}
	void SoftwareVideoManager::Rasterize(Mesh* pMesh)
	{
		for(int i =0; i< pMesh->m_TriangleNum; i++)
		{
			//true�� �ø�...
			if(pMesh->m_pBackFaceCulling[i])
				continue;
			
			if(m_ApplyModule.m_FillMode == JSS_FILL_WIRE)
			{
				Vector2i v1,v2,v3;
				v1._x= (int)pMesh->m_pTriangleList[i].m_vt[0].m_XYZ._x;
				v1._y= (int)pMesh->m_pTriangleList[i].m_vt[0].m_XYZ._y;

				v2._x= (int)pMesh->m_pTriangleList[i].m_vt[1].m_XYZ._x;
				v2._y= (int)pMesh->m_pTriangleList[i].m_vt[1].m_XYZ._y;

				v3._x= (int)pMesh->m_pTriangleList[i].m_vt[2].m_XYZ._x;
				v3._y= (int)pMesh->m_pTriangleList[i].m_vt[2].m_XYZ._y;

				m_RasterizeModule.LineDraw(v1,v2);
				m_RasterizeModule.LineDraw(v2,v3);
				m_RasterizeModule.LineDraw(v3,v1);
			}
			else if(m_ApplyModule.m_FillMode == JSS_FILL_SOLID)
			{
				//���⼭ material ������...

				float ** pZbuffer;
				Texture* pTexture;

				//�ؽ��� �����ϳ� ����...
				if(m_ApplyModule.m_TextrueApply)
					 pTexture = GetTexture(m_NowMaterial);
				else
					pTexture = NULL;

				//z-buffer �����ϳ� ����...
				if(m_ApplyModule.m_Zbuffering)
					pZbuffer = m_ppZBuffer;
				else
					pZbuffer = NULL;
					
				
				m_RasterizeModule.SolidDraw(&(pMesh->m_pTriangleList[i]),pTexture,pZbuffer);
			}
		}
	}

	void SoftwareVideoManager::EndDrawBefore()
	{
		SIZE szDest = {m_ScreenSize._x, m_ScreenSize._y};
		BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		POINT ptDest = {0,0},ptSrc = {0, 0};

		#ifdef DEBUGDATA
		PrintDebugData();
		#endif

		BitBlt(m_HDC,0,0,m_ScreenSize._x,m_ScreenSize._y,hMemDC,0,0,SRCCOPY);


		delete[] m_ppZBuffer[0];
		delete[] m_ppZBuffer;

	}
}