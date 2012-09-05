#include "SoftwareDevice.h"
#include <list>



namespace JssEngine
{
	//이벤트를 받기 위한 핸들러?
	struct DeviceHWND
	{
		HWND hWnd;
		SoftwareDevice* Device;
	};

	std::list<DeviceHWND*> DeviceMap;

	SoftwareDevice* getDeviceFromHWnd(HWND hWnd)
	{
		std::list<DeviceHWND*>::iterator iter= DeviceMap.begin();

		for(;iter != DeviceMap.end(); iter++ )
			if((*iter)->hWnd == hWnd)
				return (*iter)->Device;

		return 0;
	}

	SoftwareDevice::SoftwareDevice(Vector2i ScreenSize)
	:m_ScreenSize(ScreenSize)
	{
		m_bExit = false;
		m_bFullScreen = false;
		Init();
		
	}

	SoftwareDevice::~SoftwareDevice(void)
	{
		std::list<DeviceHWND*>::iterator iter= DeviceMap.begin();
		for (; iter != DeviceMap.end(); iter++)
		{
			if ((*iter)->hWnd == m_hWnd)
			{
				delete (*iter);
				DeviceMap.erase(iter);
				break;
			}
		}
	}
	void SoftwareDevice::InitDevice()
	{
		m_pVideoManager = new SoftwareVideoManager(m_hWnd,m_ScreenSize);
	}
	void SoftwareDevice::InitWindow()
	{
		m_hInst = GetModuleHandle(0);

		char lpszClass[] = "SRE";

 		WNDCLASSEX wcex;
 		wcex.cbSize			= sizeof(WNDCLASSEX);
 		wcex.style			= CS_HREDRAW | CS_VREDRAW;
 		wcex.lpfnWndProc	= WndProc;
 		wcex.cbClsExtra		= 0;
 		wcex.cbWndExtra		= 0;
 		wcex.hInstance		= m_hInst;
 		wcex.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
 		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
 		wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
 		wcex.lpszMenuName	= 0;
 		wcex.lpszClassName	= lpszClass;
 		wcex.hIconSm		= LoadIcon(NULL,IDI_APPLICATION);
 
 		RegisterClassEx(&wcex);

		DWORD style = WS_POPUP;

		if (!m_bFullScreen)
			style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		RECT rc;
		rc.left		= 0;
		rc.top		= 0;
		rc.right	= m_ScreenSize._x;
		rc.bottom	= m_ScreenSize._y;

		AdjustWindowRect(&rc, style, FALSE);

		const int realWidth = rc.right - rc.left;
		const int realHeight = rc.bottom - rc.top;

		int windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
		int windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

		if ( windowLeft < 0 )
			windowLeft = 0;
		if ( windowTop < 0 )
			windowTop = 0;	

		if (m_bFullScreen)
		{
			windowLeft = 0;
			windowTop = 0;
		}

 
  		m_hWnd=CreateWindow(lpszClass,lpszClass,style,
  			0,0,realWidth,realHeight,
  			NULL,(HMENU)NULL,m_hInst,NULL);


		ShowWindow(m_hWnd,SW_SHOW);
		UpdateWindow(m_hWnd);

		DeviceHWND *dv = new DeviceHWND();
		dv->Device = this;
		dv->hWnd = m_hWnd;

		DeviceMap.push_back(dv);
	}


	void SoftwareDevice::Init()
	{
		InitWindow();
		InitDevice();

	
	}
	bool SoftwareDevice::GetMessageListener(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
	{
		m_NowMessage.iMessage	= iMessage;
		m_NowMessage.wParam		= wParam;
		m_NowMessage.lParam		= lParam;

		static int iCullMode = JSS_CULL_CCW;
		static bool zbuffer = true;
		static int iFillMode = JSS_FILL_SOLID;

		switch(iMessage) {
			case WM_DESTROY:
				m_Continue = false;
				PostQuitMessage(0);
				return false;
			case WM_KEYDOWN:
				switch(wParam) {
			case VK_LEFT:
				iCullMode++;
				if(iCullMode == 3)
					iCullMode = 0;
				break;
				
			case VK_RIGHT:
				zbuffer = !zbuffer;
				break;
			case VK_UP:
				iFillMode ++;
				if(iFillMode ==3)
					iFillMode = 1;
				break;
			case VK_DOWN:
				break;
				}
				
				m_pVideoManager->SetRenderState(JSS_CULLMODE,iCullMode);
				m_pVideoManager->SetRenderState(JSS_ZBUFFER,zbuffer);
				m_pVideoManager->SetRenderState(JSS_FILLMODE,iFillMode);
		}

		
		return true;

	}

	bool SoftwareDevice::Run()
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_NOREMOVE) == 0)
		{}
		else if (GetMessage(&Message, NULL, 0, 0))
		{
			TranslateMessage (&Message) ;
			DispatchMessage (&Message) ;
		}
		if(!m_Continue)
			return false;

		return true;

		
	}
	
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	JssEngine::SoftwareDevice* device = NULL;
	device	= JssEngine::getDeviceFromHWnd(hWnd);

	if(device == NULL)
		return(DefWindowProc(hWnd,iMessage,wParam,lParam));


	if(device->GetMessageListener(hWnd,iMessage,wParam,lParam))
	{
		return(DefWindowProc(hWnd,iMessage,wParam,lParam));
	}

	return 0;
	
}



